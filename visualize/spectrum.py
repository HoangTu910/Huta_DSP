import numpy as np

import matplotlib.pyplot as plt

fs = 44100  # sampling rate

# Danh sách file và label
files = [
    ("inputSignal.txt", "Input Signal 1"),
    ("outputSignal_1.txt", "Output Signal 1"),
    # ("outputSignal_2.txt", "Output Signal 2"),
    # ("outputSignal_3.txt", "Output Signal 3"),
    # ("outputSignal_4.txt", "Output Signal 4"),
    ("outputSignal_5.txt", "Output Signal 5"),
    ("outputSignal_6.txt", "Output Signal 6"),
    ("outputSignal_7.txt", "Output Signal 7"),
]

plt.figure(figsize=(14, 10))

for i, (filename, label) in enumerate(files, start=1):
    # Đọc dữ liệu
    data = np.loadtxt(filename)
    amp = data[:, 1]

    # Tính FFT
    N = len(amp)
    freq = np.fft.rfftfreq(N, d=1/fs)
    spectrum = np.abs(np.fft.rfft(amp))

    # Vẽ phổ tần số
    plt.subplot(len(files), 1, i)
    plt.plot(freq, spectrum, label=label)
    plt.xlabel("Frequency (Hz)")
    plt.ylabel("Magnitude")
    plt.title(f"Frequency Spectrum of {label}")
    plt.grid(True)
    plt.legend()
    plt.xlim(0, 2000)

plt.tight_layout()
plt.savefig("spectrum.png")
plt.show()