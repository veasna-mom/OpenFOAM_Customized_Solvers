# OpenFOAM PCM Solver

## Overview

This repository contains **customized solvers for OpenFOAM** developed for simulating **phase change materials (PCM)** using the **Enthalpy–Porosity method**. 

The solver includes **Gaussian smoothing**, **Heaviside liquid fraction formulation**, and **cosine kernel smoothing** techniques to improve numerical stability during melting and solidification simulations.



The solver framework is implemented using the **finite volume method (FVM)** and is intended for **computational fluid dynamics (CFD) research, solver development, and educational purposes**.

---

# Physics of Phase Change

During melting or solidification, a material absorbs or releases **latent heat** while maintaining nearly constant temperature.

The **total enthalpy formulation** used in the solver is:

$$H = h + \gamma L$$

where:
* $H$ : total enthalpy
* $h$ : sensible enthalpy
* $L$ : latent heat
* $\gamma$ : liquid fraction

The liquid fraction satisfies:

$$0 \le \gamma \le 1$$

* $\gamma = 0$ → solid
* $\gamma = 1$ → liquid

---

# Liquid Fraction Formulations

## 1. Gaussian Liquid Fraction

To obtain a smooth transition between solid and liquid phases, the liquid fraction can be defined using the **Gaussian error function**:

$$\gamma = \frac{1}{2} \left[ 1 + \text{erf} \left( \frac{4(T-T_m)}{T_{mr}} \right) \right]$$

where:
* $T_m$ : melting temperature
* $T_{mr}$ : mushy temperature range

The enthalpy then becomes:

$$H = h + L \frac{1}{2} \left[ 1 + \text{erf} \left( \frac{4(T-T_m)}{T_{mr}} \right) \right]$$

This formulation provides **smooth interface capturing** and improves numerical stability.

---

## 2. Heaviside Liquid Fraction

A simpler formulation uses the **Heaviside step function**:

$$\gamma = \begin{cases} 0 & T < T_m \\ 1 & T \ge T_m \end{cases}$$

The corresponding enthalpy formulation becomes:

$$H = h + L \cdot \mathcal{H}(T-T_m)$$

where $\mathcal{H}(\cdot)$ is the **Heaviside function**.

While simple, this formulation can introduce **numerical oscillations near the phase interface**.

---

## 3. Cosine Kernel Smoothing

To avoid discontinuities, a **cosine smoothing kernel** can be used to regularize the liquid fraction:

$$\gamma = \frac{1}{2} \left[ 1 + \cos\left( \pi \frac{T-T_m}{T_{mr}} \right) \right]$$

for:

$$|T-T_m| < \frac{T_{mr}}{2}$$

and:

$$\gamma = \begin{cases} 0 & T < T_m - T_{mr}/2 \\ 1 & T > T_m + T_{mr}/2 \end{cases}$$

This smoothing improves **numerical stability and convergence**.

---

# Governing Equations

The solver resolves the conservation equations of **mass, momentum, and energy**.

### Continuity Equation

$$\nabla \cdot \mathbf{u} = 0$$

---

### Momentum Equation

$$\rho \left( \frac{\partial \mathbf{u}}{\partial t} + \mathbf{u}\cdot\nabla\mathbf{u} \right) = -\nabla p + \mu \nabla^2 \mathbf{u} + \rho \mathbf{g} + S_{Darcy}$$

The **Darcy damping term** suppresses velocity in solid regions:

$$S_{Darcy} = -A_{mush} \frac{(1-\gamma)^2}{\gamma^3+\epsilon} \mathbf{u}$$

---

### Energy Equation

$$\rho c_p \left( \frac{\partial T}{\partial t} + \mathbf{u}\cdot\nabla T \right) = k\nabla^2 T - \rho L \frac{\partial \gamma}{\partial t}$$

---

# Numerical Method

The governing equations are solved using the **finite volume method (FVM)** with the following solution procedure:

1. Solve momentum equation
2. Solve pressure correction (**PISO** algorithm)
3. Update velocity field
4. Solve temperature equation
5. Update liquid fraction $\gamma$
6. Apply Darcy damping in solid regions

---

# Installation

Clone the repository:

``bash
git clone [https://github.com/veasna-mom/OpenFOAM_Customized_Solvers.git](https://github.com/veasna-mom/OpenFOAM_Customized_Solvers.git)

# References

1. Brent, A. D., Voller, V. R., Reid, K. J. (1988).  
   *Enthalpy–Porosity Technique for Modeling Convection–Diffusion Phase Change.*  
   Numerical Heat Transfer.

2. Voller, V. R., Prakash, C. (1987).  
   *A Fixed Grid Numerical Modeling Methodology for Phase Change Problems.*  
   International Journal of Heat and Mass Transfer.

3. Gau, C., Viskanta, R. (1986).  
   *Melting and Solidification of a Pure Metal on a Vertical Wall.*  
   Journal of Heat Transfer.

---

# License

This project is intended for **research and educational purposes**.
