# from gtts import gTTS

# tts = gTTS("In 300 meters, turn left. Be prepared to stop.", lang="en")
# tts.save("assistance.mp3")

from pydub import AudioSegment

# Load file mp3
sound = AudioSegment.from_mp3("../sound/notif.mp3")

# Ghép đôi
doubled = sound + sound

# Xuất file mới
doubled.export("../sound/notif.mp3", format="mp3")

print("Done! File đã được extend gấp đôi bằng repeat.")

