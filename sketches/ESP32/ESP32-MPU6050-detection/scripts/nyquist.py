import numpy as np
import matplotlib.pyplot as plt

#a 30 hz sine sampled at different rates
duration = 0.5 # half a second
t_truth = np.linspace(0, duration, 5000)
truth = np.sin(2 * np.pi * 30 * t_truth)

fig, axes = plt.subplots(3, 1, figsize=(12, 8), sharex=True)
for ax, fs in zip(axes, [200, 100, 40]):
    t_s = np.arange(0, duration, 1/fs)
    s = np.sin(2 * np.pi * 30 * t_s)
    ax.plot(t_truth, truth, 'b-', alpha=0.4, label="True 30hz signal")
    ax.plot(t_s, s, 'ro-', markersize=5, label=f"Sampled at {fs} Hz")
    ax.set_title(f"Sample rate {fs} Hz (Nyquist - {fs/2} Hz)")
axes[-1].set_xlabel("Times (s)")
plt.tight_layout()
plt.show()