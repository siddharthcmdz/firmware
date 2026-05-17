import numpy as np
import matplotlib.pyplot as plt

t_continuous = np.linspace(0, 1, 1000) #1000 points - high resolution reality

signal_continuous = np.sin(2 * np.pi * 5 * t_continuous)


#sample at 100hz
fs_sample = 100
t_sampled = np.arange(0, 1, 1/fs_sample) #100 points
signal_sampled = np.sin(2 * np.pi * 5 * t_sampled)

plt.figure(figsize=(12, 4))
plt.plot(t_continuous, signal_continuous, 'b-', alpha=0.5, label="continuous[reality]")
plt.plot(t_sampled, signal_sampled, 'ro', markersize=4, label="Sampled (what we see)")
plt.xlabel("Time (s)"); plt.ylabel("Amplitude")
plt.title("A 5Hz sine wave sampled at 100 Hz")
plt.legend(); plt.grid(True)
plt.show()
