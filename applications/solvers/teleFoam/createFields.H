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

Info<< " Reading Transport Properties \n"<<endl;

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
    dimensionedScalar c(transportProperties.lookup("c"));
    dimensionedScalar tau(transportProperties.lookup("tau"));
    dimensionedScalar DT = sqr(c);

Info << "Wave speed c = " << c.value() << " m/s\n"
     << "c² = " << DT.value() << " m²/s²\n"
     << "Damping constant τ = " << tau.value() << " s\n" << endl;
	


//#include "createFvOptions.H"
