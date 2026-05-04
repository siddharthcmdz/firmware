"""
view_session.py — load a CSV session, plot the 6 channels + magnitude.

Usage:
    python view_session.py PATH_TO_CSV
"""

import argparse
import pathlib
import matplotlib.pyplot as plt
import numpy as np
import pandas as pd

def load_session(path):
    """Load a session csv, skipping any '#'prefixed metadata lines"""
    df = pd.read_csv(path, comment="#")
    df["t_s"] = (df["t_ms"] - df["t_ms"].iloc[0]) / 1000.0
    df["a_mag_g"] = np.sqrt(df["ax_g"]**2 + df["ay_g"]**2 + df["az_g"]**2)
    df["g_mag_dps"] = np.sqrt(df["gx_dps"]**2 +df["gy_dps"]**2 + df["gz_dps"]**2)
    
    return df

def plot_session(df, title="Session"):
    fig, axes = plt.subplots(3, 1, figsize=(12, 8), sharex=True)

    #top: 3 axis - acceleration
    axes[0].plot(df["t_s"], df["ax_g"], label="ax", linewidth=0.8)
    axes[0].plot(df["t_s"], df["ay_g"], label="ay", linewidth=0.8)
    axes[0].plot(df["t_s"], df["az_g"], label="az", linewidth=0.8)
    axes[0].set_ylabel("Acceleration (g)")
    axes[0].axhline(0, color="gray", linewidth=0.4)
    axes[0].axhline(1, color="gray", linewidth=0.4, linestyle="--")
    axes[0].legend(loc="upper right")
    axes[0].set_title(title)
    axes[0].grid(True, alpha=0.3)

    # Middle: 3-axis gyroscope
    axes[1].plot(df["t_s"], df["gx_dps"], label="gx", linewidth=0.8)
    axes[1].plot(df["t_s"], df["gy_dps"], label="gy", linewidth=0.8)
    axes[1].plot(df["t_s"], df["gz_dps"], label="gz", linewidth=0.8)
    axes[1].set_ylabel("Angular rate (°/s)")
    axes[1].legend(loc="upper right")
    axes[1].grid(True, alpha=0.3)

    # Bottom: magnitudes
    axes[2].plot(df["t_s"], df["a_mag_g"], label="|a|", color="C3")
    axes[2].axhline(1, color="gray", linewidth=0.4, linestyle="--", label="1g rest")
    axes[2].axhline(0.4, color="orange", linewidth=0.6, linestyle=":", label="freefall threshold")
    axes[2].axhline(2.0, color="red", linewidth=0.6, linestyle=":", label="impact threshold")
    axes[2].set_ylabel("|a| (g)")
    axes[2].set_xlabel("Time (s)")
    axes[2].legend(loc="upper right")
    axes[2].grid(True, alpha=0.3)

    plt.tight_layout()
    plt.show()
                            

if __name__ == "__main__":
    p = argparse.ArgumentParser()
    p.add_argument("--path", type=pathlib.Path)
    args = p.parse_args()
    df = load_session(args.path)
    title = args.path.stem
    plot_session(df, title = title)