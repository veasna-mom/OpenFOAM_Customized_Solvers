Info<< "Reading field T\n" << endl;
volScalarField T
(
    IOobject
    (
        "T",
        runTime.timeName(),
        mesh,
        IOobject::MUST_READ,
        IOobject::AUTO_WRITE
    ),
    mesh
);


Info<< "Reading field p_rgh\n" << endl;
volScalarField p_rgh
(
    IOobject
    (
        "p_rgh",
        runTime.timeName(),
        mesh,
        IOobject::MUST_READ,
        IOobject::AUTO_WRITE
    ),
    mesh
);


Info<< "Reading field U\n" << endl;
volVectorField U
(
    IOobject
    (
        "U",
        runTime.timeName(),
        mesh,
        IOobject::MUST_READ,
        IOobject::AUTO_WRITE
    ),
    mesh
);


#include "createPhi.H"


// =============================
// Transport properties
// =============================

Info<< "Reading transport properties\n" << endl;

IOdictionary transportProperties
(
    IOobject
    (
        "transportProperties",
        runTime.constant(),
        mesh,
        IOobject::MUST_READ,
        IOobject::NO_WRITE
    )
);


// =============================
// PCM properties
// =============================

dimensionedScalar Tsolid( "Tsolid",dimTemperature,transportProperties);

dimensionedScalar Tliquid( "Tliquid", dimTemperature,transportProperties);

// Melt temperature midpoint
dimensionedScalar Tm = 0.5*(Tsolid + Tliquid);

// Melt range
dimensionedScalar Tmr = (Tliquid - Tsolid);


// Latent heat J/kg
dimensionedScalar hm ("hm", dimensionSet(0,2,-2,0,0,0,0),transportProperties);


// Specific heat J/(kg K)
dimensionedScalar Cp
(
    "Cp",
    dimensionSet(0,2,-2,-1,0,0,0),
    transportProperties
);


// Thermal expansion coefficient 1/K
dimensionedScalar beta("beta", dimensionSet(0,0,0,-1,0,0,0), transportProperties);


dimensionedScalar TRef("TRef", dimTemperature,transportProperties);


// Mushy zone constant  with dimensions of inverse time, used to control the velocity in the mushy zone
dimensionedScalar Amush( "Amush", dimensionSet(0,0,-1,0,0,0,0),transportProperties);
// Prandtl numbers
dimensionedScalar Pr("Pr",  dimless, transportProperties);

dimensionedScalar Prt( "Prt", dimless, transportProperties);


// =============================
// Liquid fraction field
// =============================

volScalarField gamma
(
    IOobject
    (
        "gamma",
        runTime.timeName(),
        mesh,
        IOobject::READ_IF_PRESENT,
        IOobject::AUTO_WRITE
    ),
    mesh,
    dimensionedScalar("gamma", dimless, 0.0)
);
dimensionedScalar Epsilon( "Epsilon", dimless, transportProperties); // Small number to avoid division by zero in mushy coefficient

// =============================
// Turbulence model
// =============================

singlePhaseTransportModel laminarTransport(U, phi);

Info<< "Creating turbulence model\n" << endl;

autoPtr<incompressible::turbulenceModel> turbulence
(
    incompressible::turbulenceModel::New(U, phi, laminarTransport)
);


// =============================
// Boussinesq density
// =============================

volScalarField rhok
(
    IOobject
    (
        "rhok",
        runTime.timeName(),
        mesh
    ),
    1.0 - beta*(T - TRef)
);


// =============================
// Turbulent thermal diffusivity
// =============================

volScalarField alphat
(
    IOobject
    (
        "alphat",
        runTime.timeName(),
        mesh,
        IOobject::NO_READ,
        IOobject::AUTO_WRITE
    ),
    turbulence->nut()/Prt
);


#include "readGravitationalAcceleration.H"
#include "readhRef.H"
#include "gh.H"


// =============================
// Pressure
// =============================

volScalarField p
(
    IOobject
    (
        "p",
        runTime.timeName(),
        mesh,
        IOobject::NO_READ,
        IOobject::AUTO_WRITE
    ),
    p_rgh + rhok*gh
);


label pRefCell = 0;
scalar pRefValue = 0.0;

setRefCell
(
    p,
    p_rgh,
    pimple.dict(),
    pRefCell,
    pRefValue
);


if (p_rgh.needReference())
{
    p += dimensionedScalar
    (
        "p",
        p.dimensions(),
        pRefValue - getRefCellValue(p, pRefCell)
    );
}


mesh.setFluxRequired(p_rgh.name());
/*
dimensionedScalar mushyCoeff
(
    "mushyCoeff",
    dimensionSet(0,0,-1,0,0,0,0),
    Amush*pow(1.0 -gamma,2.0)/(pow(gamma,3.0) + Epsilon)
);
*/

//Include mathematical constants
#include "mathematicalConstants.H"
#include "createMRF.H"
#include "createIncompressibleRadiationModel.H"
#include "createFvOptions.H"