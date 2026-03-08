    // Solve the momentum equation

   // Darcy Term

   volScalarField mushyCoeff
   (
    IOobject
    (
        "mushyCoeff",
        runTime.timeName(),
        mesh,
        IOobject::NO_READ,
        IOobject::NO_WRITE
    ),
    Amush*pow(1.0 -gamma,2.0)/(pow(gamma,3.0) + Epsilon)
   );

    MRF.correctBoundaryVelocity(U);

    fvVectorMatrix UEqn
    (
        fvm::ddt(U) + fvm::div(phi, U)
      + MRF.DDt(U)
      + turbulence->divDevReff(U)
      + fvm::Sp(mushyCoeff, U)   // darcy term
      ==
        fvOptions(U)
    );

    UEqn.relax();

    fvOptions.constrain(UEqn);

    if (pimple.momentumPredictor())
    {
        solve
        (
            UEqn
         ==
            fvc::reconstruct
            (
                (
                  - ghf*fvc::snGrad(rhok)
                  - fvc::snGrad(p_rgh)
                )*mesh.magSf()
            )
        );

        fvOptions.correct(U);
    }
