

import numpy as np
import matplotlib.pyplot as plt

fs = 100 #frequency
duration = 2.0 #seconds
sample_period = 0.01 #10/1000

t = np.arange(0, duration, 1/fs)
signal = np.sin(2 * np.pi * 2 * t) # 2hz sine wave

n = np.arange(0, fs * duration, 1)
signal_n = np.sin(2 * np.pi * 2 * sample_period * n)

plt.figure(figsize=(10, 3))

# plt.plot(t, signal)
plt.plot(t, signal_n)

plt.xlabel("Time (s)")
plt.ylabel("Amplitude")
plt.grid(True)
plt.show()
