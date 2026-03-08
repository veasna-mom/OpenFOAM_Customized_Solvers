Info<< "Reading thermophysical properties\n" << endl;

// ----------------------------------------------------
// Primary Fields
// ----------------------------------------------------

volScalarField T
(
    IOobject("T", runTime.timeName(), mesh, IOobject::MUST_READ, IOobject::AUTO_WRITE),
    mesh
);

volScalarField p_rgh
(
    IOobject("p_rgh", runTime.timeName(), mesh, IOobject::MUST_READ, IOobject::AUTO_WRITE),
    mesh
);

volVectorField U
(
    IOobject("U", runTime.timeName(), mesh, IOobject::MUST_READ, IOobject::AUTO_WRITE),
    mesh
);

#include "createPhi.H"

// ----------------------------------------------------
// Transport Properties
// ----------------------------------------------------

IOdictionary transportProperties
(
    IOobject
    (
        "transportProperties",
        runTime.constant(),
        mesh,
        IOobject::MUST_READ_IF_MODIFIED,
        IOobject::NO_WRITE
    )
);

singlePhaseTransportModel laminarTransport(U, phi);

// ----------------------------------------------------
// Phase Change Properties
// ----------------------------------------------------

dimensionedScalar rho("rho", dimDensity, transportProperties);
dimensionedScalar Ts("Ts", dimTemperature, transportProperties);
dimensionedScalar Tl("Tl", dimTemperature, transportProperties);
dimensionedScalar Cp("Cp", dimEnergy/(dimMass*dimTemperature), transportProperties);
dimensionedScalar L("L", dimEnergy/dimMass, transportProperties);
dimensionedScalar beta("beta", dimensionSet(0,0,0,-1,0,0,0), transportProperties);
dimensionedScalar Pr("Pr", dimless, transportProperties);
dimensionedScalar Prt("Prt", dimless, transportProperties);
dimensionedScalar Amush("Amush", dimensionSet(0,0,-1,0,0,0,0), transportProperties);
dimensionedScalar epsilon("epsilon", dimless, transportProperties);
dimensionedScalar TRef("TRef", dimTemperature, transportProperties);



// ----------------------------------------------------
// Safety Checks
// ----------------------------------------------------

dimensionedScalar deltaTm = Tl - Ts;

if (mag(deltaTm.value()) < SMALL)
{
    FatalErrorInFunction
        << "Ts and Tl too close. Phase change range must be finite."
        << exit(FatalError);
}

if (Prt.value() <= SMALL)
{
    FatalErrorInFunction
        << "Prt must be > 0"
        << exit(FatalError);
}

// ----------------------------------------------------
// Turbulence
// ----------------------------------------------------

autoPtr<incompressible::turbulenceModel> turbulence
(
    incompressible::turbulenceModel::New(U, phi, laminarTransport)
);

/// ----------------------------------------------------
// Phase Change & AHC Fields
// ----------------------------------------------------
/*
// Liquid Fraction
volScalarField gamma
(
    IOobject("gamma", runTime.timeName(), mesh, IOobject::READ_IF_PRESENT, IOobject::AUTO_WRITE),
    min(scalar(1), max(scalar(0), (T - Ts)/deltaTm))
);
*/

// Liquid Fraction
volScalarField gamma
(
    IOobject("gamma", runTime.timeName(), mesh, IOobject::READ_IF_PRESENT, IOobject::AUTO_WRITE),
    mesh,
    dimensionedScalar("zero", dimless, 0.0)
);

/// This avoids the (a-b) dimension error during the start of the simulation
gamma = (T - Ts)/deltaTm;
gamma = max(scalar(0), min(scalar(1), gamma));


// Effective Heat Capacity (Total Energy Density derivative)
volScalarField CpEff
(
    IOobject("CpEff", runTime.timeName(), mesh, IOobject::NO_READ, IOobject::AUTO_WRITE),
    mesh,
    Cp
);

// Derivative of gamma w.r.t Temperature (the "Phase Change Spike")
volScalarField dGdT
(
    IOobject("dGdT", runTime.timeName(), mesh, IOobject::NO_READ, IOobject::NO_WRITE),
    mesh,
    dimensionedScalar("zero", dimensionSet(0,0,0,-1,0,0,0), 0.0)
);

// ----------------------------------------------------
// Buoyancy & Turbulence
// ----------------------------------------------------

volScalarField rhok
(
    IOobject("rhok", runTime.timeName(), mesh, IOobject::NO_READ, IOobject::NO_WRITE),
    1.0 - beta*(T - TRef)
);

volScalarField alphat
(
    IOobject("alphat", runTime.timeName(), mesh, IOobject::NO_READ, IOobject::AUTO_WRITE),
    mesh,
    dimensionedScalar("alphat", dimensionSet(0,2,-1,0,0,0,0), 0.0)
);
#include "readGravitationalAcceleration.H"
#include "readhRef.H"
#include "gh.H"

volScalarField p
(
    IOobject("p", runTime.timeName(), mesh, IOobject::NO_READ, IOobject::AUTO_WRITE),
    p_rgh + rhok*gh
);

label pRefCell = 0;
scalar pRefValue = 0.0;
setRefCell(p, p_rgh, pimple.dict(), pRefCell, pRefValue);

mesh.setFluxRequired(p_rgh.name());
volScalarField H
(
    IOobject
    (
        "H",
        runTime.timeName(),
        mesh,
        IOobject::NO_READ,
        IOobject::AUTO_WRITE
    ),
    Cp*T + L*gamma
);
#include "createMRF.H"
#include "createIncompressibleRadiationModel.H"
#include "createFvOptions.H"
#include "mathematicalConstants.H"