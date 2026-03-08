for (int i=0; i<10; i++)
{
    alphat = turbulence->nut()/Prt;
    alphat.correctBoundaryConditions();

    volScalarField alphaEff
    (
        "alphaEff",
        turbulence->nu()/Pr + alphat
    );

    volScalarField latentCoeff
    (
        "latentCoeff",
        hm/Cp * 4.0/Tmr
      / Foam::sqrt(Foam::constant::mathematical::pi)
      * exp(-pow(4.0*(T-Tm)/Tmr,2))
    );

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

    gamma = 0.5 * Foam::erf( 4.0*(T-Tm)/Tmr ) + 0.5;
    
    // Relax the liquid fraction field to avoid numerical issues in the mushy zone
    gamma = 0.7*gamma + 0.3*gamma.oldTime();
}