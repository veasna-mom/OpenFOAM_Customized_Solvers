for (int i=0; i<10; i++)
{
    alphat = turbulence->nut()/Prt;
    alphat.correctBoundaryConditions();

    volScalarField alphaEff
    (
        "alphaEff",
        turbulence->nu()/Pr + alphat
    );
/// Using Cosine Kernel for the Latent Heat Source Term
   //volScalarField latentCoeff ("latentCoeff",     )
dimensionedScalar eps = Tmr/2.0;
const scalar PI = Foam::constant::mathematical::pi;

// Normalized temperature
volScalarField theta = (T - Tm)/eps;

// Latent heat coefficient
volScalarField latentCoeff
(
    IOobject
    (
        "latentCoeff",
        runTime.timeName(),
        mesh,
        IOobject::NO_READ,
        IOobject::NO_WRITE
    ),
    mesh,
    dimensionedScalar("zero", dimless, 0.0)
);

latentCoeff =
pos(1.0 - mag(theta))
*
(
    (hm/Cp)
  * (1.0/(2.0*eps))
  * (1.0 + cos(PI*theta))
);



/*
    volScalarField latentCoeff
    (
        "latentCoeff",
        hm/Cp * 4.0/Tmr
      / Foam::sqrt(Foam::constant::mathematical::pi)
      * exp(-pow(4.0*(T-Tm)/Tmr,2))
    );
*/
    fvScalarMatrix TEqn
    (
        (1.0 + latentCoeff)*fvm::ddt(T)
      + fvm::div(phi,T)
      - fvm::laplacian(alphaEff,T)
    );

    TEqn.relax();

    fvOptions.constrain(TEqn);

    TEqn.solve();

    rhok = 1.0 - beta*(T - TRef);

    //gamma = 0.5 * Foam::erf( 4.0*(T-Tm)/Tmr ) + 0.5;
// Liquid fraction
gamma =
pos(theta - 1.0)
+ 0.5*(1.0 + theta + sin(PI*theta)/PI)
  * pos(1.0 - mag(theta));

gamma *= pos(theta + 1.0);

// Relaxation
gamma = 0.7*gamma + 0.3*gamma.oldTime();


    // Relax the liquid fraction field to avoid numerical issues in the mushy zone
    gamma = 0.7*gamma + 0.3*gamma.oldTime();
}