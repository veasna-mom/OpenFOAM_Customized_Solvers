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


 Info<<" Reading transportProperties \n"<<endl;
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
	Info<<" Reading Diffusivity DT \n"<<endl;
	dimensionedScalar c
	(
	 "c",
	 dimVelocity,
	 transportProperties
	);
     
    dimensionedScalar DT = sqr(c);
	
	
	//Print Information About C and DT
	
	Info<<" Value of c is = "<<c.value()<<"m/s"<<endl;
	Info<<" Value of DT is ="<<DT.value()<<"m^2/s^2 \n"<<endl;

//#include "createFvOptions.H"
