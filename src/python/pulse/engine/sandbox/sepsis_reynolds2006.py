import numpy as np
import pathlib
from scipy.integrate import solve_ivp
import matplotlib.pyplot as plt

k_pm = 0.6
k_mp = 0.01
s_m = 0.005
miu_m = 0.002
P_inf = 20
k_pn = 1.8
k_np = 0.1
k_nn = 0.01
s_nr = 0.08
miu_nr = 0.12
miu_n = 0.05
k_nd = 0.02
k_dn = 0.35
x_dn = 0.06
miu_d = 0.02
C_inf = 0.28
s_c = 0.0125
k_cn = 0.04
k_cnd = 48.0
miu_c = 0.1

q_na = 6


def g(x, C_A):
    return x / (1 + (C_A / C_inf) ** 2)


def g_s(x):
    return x**6 / (x_dn ** 6 + x**6)


def dPdt(t, P, N_star, D, C_A):
    return (k_pg * P * (1 - P / P_inf) - k_pm * s_m * P / (miu_m + k_mp * P) - k_pn * g(N_star, C_A) * P)

def dN_stardt(t, P, N_star, D, C_A):
    R = g(k_nn * N_star + k_np * P + k_nd * D, C_A)
    return (s_nr * R / (miu_nr + R) - miu_n * N_star)


def dDdt(t, P, N_star, D, C_A):
    return (k_dn * g_s(g(N_star, C_A)) - miu_d * D)
    # in 2019 paper dDdt is a function of stroke volume S_l, and S_l is a function of N_star


def dC_Adt(t, P, N_star, D, C_A):
    f = g(N_star + k_cnd * D, C_A)
    return (s_c + k_cn * f / (1 + f) - miu_c * C_A)

def dFdt(t, y):
    P = y[0]
    N_star = y[1]
    D= y[2]
    C_A = y[3]
    return [dPdt(t, P, N_star, D, C_A),
            dN_stardt(t, P, N_star, D, C_A),
            dDdt(t, P, N_star, D, C_A),
            dC_Adt(t, P, N_star, D, C_A)]

def plot(sol, name):

    print(sol.t)
    print(sol.y)
    print(sol.y[0, -1])

    path = pathlib.Path(f"./test_results/sepsis_reynolds2006/{name}")
    path.mkdir(parents=True, exist_ok=True)

    plt.figure()
    plt.plot(sol.t, sol.y[0, :].T)
    plt.xlabel('hours')
    plt.legend(['P'], shadow=True)
    plt.title(f"{name} - Pathogen Count")
    plt.savefig(str(path)+"/PathogenCountvsTime.jpg")
    plt.close()

    plt.figure()
    plt.plot(sol.t, sol.y[1, :].T)
    plt.xlabel('hours')
    plt.legend(['N_star'], shadow=True)
    plt.title(f"{name} - Activated Phagocytes")
    plt.savefig(str(path)+"/ActivatedPhagocytesvsTime.jpg")
    plt.close()

    plt.figure()
    plt.plot(sol.t, sol.y[2, :].T)
    plt.xlabel('hours')
    plt.legend(['D'], shadow=True)
    plt.title(f"{name} - Tissue Damage")
    plt.savefig(str(path)+"/TissueDamagevsTime.jpg")
    plt.close()

    plt.figure()
    plt.plot(sol.t, sol.y[3, :].T)
    plt.xlabel('hours')
    plt.legend(['C_A'], shadow=True)
    plt.title(f"{name} - Anti-inflammatory Mediator")
    plt.savefig(str(path)+"/AntiInflammatoryMediatorsvsTime.jpg")
    plt.close()


if __name__ == "__main__":

    # Values used here are to match the plots in this paper
    # https://www.mathematics.pitt.edu/sites/default/files/research-pdfs/reynolds.pdf

    # input
    # y0 = [P_0, N_0, D_0, C_A_0]
    # y0 = [count of pathogen, activated phagocytes, tissue damage, anti-inflammatory mediator]

    # Simulate 12 days, with 14 days as the eval edge
    hours = 288  # Number of hours to simulate
    t_eval_max = 14 * 24  # Numbers of hours to hold (gives us a nice pad in our plots)

    k_pg = 0.25  # growth rate of pathogen
    sol = solve_ivp(dFdt, [0, hours], [1.0, 0, 0, 0.125], t_eval=np.linspace(0, hours, t_eval_max))
    plot(sol, "MildInfectionMildProgressionSepsis")

    k_pg = 0.33  # growth rate of pathogen
    sol = solve_ivp(dFdt, [0, hours], [1.0, 0, 0, 0.125], t_eval=np.linspace(0, hours, t_eval_max))
    plot(sol, "MildInfectionModerateProgressionSepsis")

    k_pg = 0.6  # growth rate of pathogen
    sol = solve_ivp(dFdt, [0, hours], [1.0, 0, 0, 0.125], t_eval=np.linspace(0, hours, t_eval_max))
    plot(sol, "MildInfectionSevereProgressionSepsis")

    k_pg = 0.25  # growth rate of pathogen
    sol = solve_ivp(dFdt, [0, hours], [1.25, 0, 0, 0.125], t_eval=np.linspace(0, hours, t_eval_max))
    plot(sol, "ModerateInfectionMildProgressionSepsis")

    k_pg = 0.33  # growth rate of pathogen
    sol = solve_ivp(dFdt, [0, hours], [1.25, 0, 0, 0.125], t_eval=np.linspace(0, hours, t_eval_max))
    plot(sol, "ModerateInfectionModerateProgressionSepsis")

    k_pg = 0.6  # growth rate of pathogen
    sol = solve_ivp(dFdt, [0, hours], [1.25, 0, 0, 0.125], t_eval=np.linspace(0, hours, t_eval_max))
    plot(sol, "ModerateInfectionSevereProgressionSepsis")

    k_pg = 0.25  # growth rate of pathogen
    sol = solve_ivp(dFdt, [0, hours], [1.4, 0, 0, 0.125], t_eval=np.linspace(0, hours, t_eval_max))
    plot(sol, "SevereInfectionMildProgressionSepsis")

    k_pg = 0.33  # growth rate of pathogen
    sol = solve_ivp(dFdt, [0, hours], [1.4, 0, 0, 0.125], t_eval=np.linspace(0, hours, t_eval_max))
    plot(sol, "SevereInfectionModerateProgressionSepsis")

    k_pg = 0.6  # growth rate of pathogen
    sol = solve_ivp(dFdt, [0, hours], [1.4, 0, 0, 0.125], t_eval=np.linspace(0, hours, t_eval_max))
    plot(sol, "SevereInfectionSevereProgressionSepsis")

    k_pg = 0.3  # growth rate of pathogen
    sol = solve_ivp(dFdt, [0, hours], [1.5, 0, 0, 0.125], t_eval=np.linspace(0, hours, t_eval_max))
    plot(sol, "Aseptic")

    k_pg = 0.65  # growth rate of pathogen
    sol = solve_ivp(dFdt, [0, hours], [1.5, 0, 0, 0.125], t_eval=np.linspace(0, hours, t_eval_max))
    plot(sol, "MaximumSepsis")



