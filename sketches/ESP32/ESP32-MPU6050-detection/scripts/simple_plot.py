

import numpy as np
import matplotlib.pyplot as plt

fs = 100 #frequency
duration = 1.0 #seconds
t = np.arange(0, duration, 1/fs)

signal = np.sin(2 * np.pi * 2 * t)

plt.figure(figsize=(10, 3))
plt.plot(t, signal)
plt.xlabel("Time (s)")
plt.ylabel("Amplitude")
plt.grid(True)
plt.show()
