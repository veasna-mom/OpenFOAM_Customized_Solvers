// Solve the momentum equation
MRF.correctBoundaryVelocity(U);

// --- Compute Mushy Zone (Darcy) Coefficient ---
// This term becomes massive as gamma -> 0 (Solid), forcing U to zero.
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
    Amush * sqr(1.0 - gamma) / (pow(gamma, 3.0) + Epsilon)
);

fvVectorMatrix UEqn
(
    fvm::ddt(U) 
  + fvm::div(phi, U)
  + MRF.DDt(U)
  + turbulence->divDevReff(U)
  + fvm::Sp(mushyCoeff, U) // Implicit sink term
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
            ) * mesh.magSf()
        )
    );

    fvOptions.correct(U);
}