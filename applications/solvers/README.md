# OpenFOAM PCM Solver

## Overview

This repository contains **customized solvers for OpenFOAM** developed for simulating **phase change materials (PCM)** using the **Enthalpy–Porosity method**.

The solver includes a **Gaussian liquid fraction smoothing formulation** to improve numerical stability during melting and solidification processes. These solvers are intended for **computational fluid dynamics (CFD) research, solver development, and educational purposes**.

The numerical framework is implemented using the **finite volume method (FVM)**.

---

# Physics of Phase Change

During melting or solidification, the material absorbs or releases **latent heat** while maintaining approximately constant temperature.

The **total enthalpy** is defined as

$$
H = h + \gamma L
$$

where

* (H) : total enthalpy
* (h) : sensible enthalpy
* (L) : latent heat
* (\gamma) : liquid fraction

The liquid fraction varies between

$$
0 \le \gamma \le 1
$$

* $(\gamma = 0)$ : solid
* $(\gamma = 1)$ : liquid

---

# Gaussian Liquid Fraction Formulation

To avoid numerical oscillations near the phase interface, the liquid fraction is defined using a **smooth Gaussian error function**:

$$
\gamma =
\frac{1}{2}
\left[
1 + \text{erf}
\left(
\frac{4(T-T_m)}{T_{mr}}
\right)
\right]
$$

where

* $(T_m)$ : melting temperature
* $(T_{mr})$ : mushy temperature range

This formulation ensures a **smooth transition between solid and liquid phases** and improves solver stability.

---

# Governing Equations

The solver resolves the conservation equations of **mass, momentum, and energy**.

### Continuity Equation

$$
\nabla \cdot \mathbf{u} = 0
$$

### Momentum Equation

$$
\rho
\left(
\frac{\partial \mathbf{u}}{\partial t}
+
\mathbf{u}\cdot\nabla\mathbf{u}
\right)
=======

-\nabla p
+
\mu \nabla^2 \mathbf{u}
+
\rho \mathbf{g}
+
S_{Darcy}
$$

The **Darcy damping term** suppresses velocity in solid regions:

$$
S_{Darcy}
=========

A_{mush}
\frac{(1-\gamma)^2}{\gamma^3+\epsilon}
\mathbf{u}
$$

---

### Energy Equation

$$
\rho c_p
\left(
\frac{\partial T}{\partial t}
+
\mathbf{u}\cdot\nabla T
\right)
=======

k\nabla^2 T
+
\rho L
\frac{\partial \gamma}{\partial t}
$$

---

# Numerical Method

The governing equations are solved using the **finite volume method (FVM)** with the following solution procedure:

1. Solve momentum equation
2. Solve pressure correction (PISO algorithm)
3. Update velocity field
4. Solve temperature equation
5. Update liquid fraction
6. Apply Darcy damping in solid regions

---

# Features

* Custom **OpenFOAM PCM solvers**
* **Porosity–enthalpy phase change modeling**
* **Gaussian smoothing for liquid fraction**
* Modified **energy equation with latent heat source**
* Suitable for **melting and solidification simulations**
* Includes **basic PDE discretization examples for beginners**

---

# Installation

Clone the repository

```
git clone https://github.com/veasna-mom/OpenFOAM_Customized_Solvers.git
```

Compile the solver

```
wmake
```

---

# Usage

Run the solver in your case directory:

```
yourCustomSolver
```

Typical applications include

* PCM melting simulations
* Thermal energy storage
* Natural convection during melting
* Numerical solver development

---

# Requirements

* OpenFOAM
* Linux / WSL environment
* Basic CFD knowledge

---

# Author

Veasna Mom
Indian Institute of Technology Bombay

---

# References

1. Brent, A.D., Voller, V.R., Reid, K.J. (1988) – *Enthalpy–Porosity Technique for Modeling Convection–Diffusion Phase Change.*

2. Voller, V.R., Prakash, C. (1987) – *A Fixed Grid Numerical Modeling Methodology for Phase Change Problems.*

3. Gau, C., Viskanta, R. (1986) – *Melting and Solidification of a Pure Metal on a Vertical Wall.*

---

# License

This project is intended for **research and educational purposes**.
