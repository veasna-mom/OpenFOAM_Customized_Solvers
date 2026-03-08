/*---------------------------------------------------------------------------*\
  =========                 |
  \\      /  F ield         | OpenFOAM: The Open Source CFD Toolbox
   \\    /   O peration     |
    \\  /    A nd           | www.openfoam.com
     \\/     M anipulation  |
-------------------------------------------------------------------------------
    Copyright (C) 2011-2017 OpenFOAM Foundation
-------------------------------------------------------------------------------
License
    This file is part of OpenFOAM.

    OpenFOAM is free software: you can redistribute it and/or modify it
    under the terms of the GNU General Public License as published by
    the Free Software Foundation, either version 3 of the License, or
    (at your option) any later version.

    OpenFOAM is distributed in the hope that it will be useful, but WITHOUT
    ANY WARRANTY; without even the implied warranty of MERCHANTABILITY or
    FITNESS FOR A PARTICULAR PURPOSE.  See the GNU General Public License
    for more details.

    You should have received a copy of the GNU General Public License
    along with OpenFOAM.  If not, see <http://www.gnu.org/licenses/>.

Application
    my_icoFoam

Group
    grpIncompressibleSolvers

Description
    incompressible NS equation solver.

    \heading Solver details
    The equation is given by:

    \f[
        \ddt{T} + \div \left(\vec{U} T\right) - \div \left(D_T \grad T \right)
        = S_{T}
    \f]

    Where:
    \vartable
        T       | Passive scalar
        D_T     | Diffusion coefficient
        S_T     | Source
    \endvartable

    \heading Required fields
    \plaintable
        T       | Passive scalar
        U       | Velocity [m/s]
    \endplaintable

\*---------------------------------------------------------------------------*/

#include "fvCFD.H"
#include "pisoControl.H"


// * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * //

int main(int argc, char *argv[])
{
    argList::addNote
    (
        "Transient Solver for Incompressible,Laminar, Newtonian Fluid ."
    );

    #include "addCheckCaseOptions.H"
    #include "setRootCaseLists.H"
    #include "createTime.H"
    #include "createMesh.H"

    pisoControl piso(mesh);

    #include "createFields.H"
	#include "initContinuityErrs.H"

    // * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * //

    Info<< "\nStarting Solving\n" << endl;

    #include "CourantNo.H"
	#include "initContinuityErrs.H" // This calculating error

    while (runTime.loop())
    {
        Info<< "Time = " << runTime.timeName() << nl << endl;

        // Velecity Predictor, Momentum Predictor Step
		// Solving Without Pressure 
		
		
            fvVectorMatrix UEqn
            (
                fvm::ddt(U)
              + fvm::div(phi, U)
              - fvm::laplacian(nu, U)
            
            );
        // Pressure 
		if (piso.momentumPredictor())
		{
			//calculating pressure gradient explicity
			solve
			(
              UEqn == -fvc::grad(p)
			);
			
		}
        //Starting our PISO Loop
		// PISO Correction
		while (piso.correct())
	    {
			volScalarField rAU(1.0/UEqn.A()); // this Matrix
			volVectorField HbyA(constrainHbyA(rAU*UEqn.H(),U,p));
			surfaceScalarField phiHbyA
			(
			 "phiHbyA",
			  fvc::flux(HbyA)
			  +fvc::interpolate(rAU)*fvc::ddtCorr(U,phi)
			
			);
			
			adjustPhi(phiHbyA,U,p);
			
			//Update the pressure BC to ensure flux consistency
			// in 
			
			constrainPressure(p,U,phiHbyA,rAU);
			
		//Non-orthogoanl corrector loop
		
		
		
		
		while(piso.correctNonOrthogonal())
		{
			
			
			// Pressure corrector

                fvScalarMatrix pEqn
                (
                    fvm::laplacian(rAU, p) == fvc::div(phiHbyA)
                );

                pEqn.setReference(pRefCell, pRefValue);

                pEqn.solve(p.select(piso.finalInnerIter()));

                if (piso.finalNonOrthogonalIter())
                {
                    phi = phiHbyA - pEqn.flux();
                }

			
		}
		
			
			
		#include "continuityErrs.H"

            U = HbyA - rAU*fvc::grad(p); //final velocity
            U.correctBoundaryConditions();
	
			
	}		
        

        runTime.write();
		runTime.printExecutionTime(Info);
    }

    Info<< "End\n" << endl;

    return 0;
}


// ************************************************************************* //
