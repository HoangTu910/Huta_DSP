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
    ("outputSignal_7.txt", "Output Signal 7"),
    ("outputSignal_8.txt", "Output Signal 8"),
]

plt.figure(figsize=(14, 10))

for i, (filename, label) in enumerate(files, start=1):
    # Đọc dữ liệu
    data = np.loadtxt(filename)
    time = data[:, 0] / fs
    amp = data[:, 1]

    # Chỉ lấy 0.0001 giây đầu
    mask = time <= 0.0000088

    # Vẽ subplot
    plt.subplot(len(files), 1, i)
    plt.plot(time[mask], amp[mask], label=label)
    plt.xlabel("Time (s)")
    plt.ylabel("Amplitude")
    plt.title(f"{label} (fs = {fs} Hz) – first 0.0001 s")
    plt.grid(True)
    plt.legend()
    
print("Done!")
plt.tight_layout()
plt.savefig("plot.png")
plt.show()