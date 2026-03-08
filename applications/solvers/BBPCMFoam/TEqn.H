{
    // --- 1. Update Phase State (Standard tracking) ---
    gamma = min(scalar(1), max(scalar(0), (T - Ts)/deltaTm));
    gamma.correctBoundaryConditions();

    // --- 2. Gaussian dGdT (The Latent Heat Spike) ---
    // Extract raw values to ensure the exponent is purely dimensionless
    const scalar Tm_val = Ts.value() + 0.5 * deltaTm.value();
    const scalar s_val = 0.25 * deltaTm.value(); // Standard deviation (width)
    const scalar invRoot2Pi = 1.0 / Foam::sqrt(2.0 * Foam::constant::mathematical::pi);

    // Calculate dGdT: [1/K]
    // We use .primitiveField() for the math to ensure no dimension conflicts in exp()
    // then assign back to the volScalarField dGdT
    dGdT.primitiveFieldRef() = (invRoot2Pi / s_val) * Foam::exp(-0.5 * Foam::sqr((T.primitiveField() - Tm_val) / s_val));
    
    dGdT.correctBoundaryConditions();

    // --- 3. Apparent Heat Capacity Multiplier (Dimensionless) ---
    // (L/Cp) has dimensions [K], dGdT has [1/K]. Product is dimensionless.
    volScalarField sigma_AHC
    (
        IOobject("sigma_AHC", runTime.timeName(), mesh),
        1.0 + (L/Cp) * dGdT
    );

    // --- 4. Thermal Diffusivity ---
    alphat = turbulence->nut()/Prt;
    alphat.correctBoundaryConditions();
    volScalarField alphaEff = (turbulence->nu()/Pr) + alphat;

    // --- 5. Solve Temperature Equation ---
    fvScalarMatrix TEqn
    (
        fvm::ddt(sigma_AHC, T)
      + fvm::div(phi * fvc::interpolate(sigma_AHC), T)
      - fvm::laplacian(alphaEff * sigma_AHC, T)
     ==
        fvOptions(T)
    );

    TEqn.relax();
    fvOptions.constrain(TEqn);
    TEqn.solve();
    fvOptions.correct(T);

    // --- 6. Buoyancy & Enthalpy ---
    rhok = 1.0 - beta*(T - TRef);
    rhok.correctBoundaryConditions();
    H = Cp*T + L*gamma; 

    Info<< "Max Boussinesq parameter: " << gMax(mag(beta*(T - TRef))()) << endl;
}