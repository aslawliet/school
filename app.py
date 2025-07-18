import os
import random
from pathlib import Path
from groq import Groq
import time

# Optional playsound fallback
try:
    from playsound import playsound
except ImportError:
    playsound = None

# Set your Groq API key
os.environ["GROQ_API_KEY"] = "your-groq-api-key"
client = Groq()

# Output file path
speech_file_path = Path(__file__).parent / "speech.mp3"

# Sample texts
texts = [
    "Hello from your phone!",
    "Python and Groq working together!",
    "This is a random message from the list.",
    "Hope you're having a great day!",
    "Speech synthesis on Android is awesome!"
]

# Pick a random phrase
selected_text = random.choice(texts)
print(f"[INFO] Selected: {selected_text}")

# Call Groq TTS
try:
    response = client.audio.speech.create(
        model="playai-tts",
        voice="Quinn-PlayAI",
        response_format="mp3",
        input=selected_text,
    )

    # Save MP3
    response.write_to_file(speech_file_path)
    print(f"[INFO] MP3 saved to: {speech_file_path}")

except Exception as e:
    print(f"[ERROR] Failed to generate TTS: {e}")
    exit(1)

# Play audio
def play_mp3(filepath):
    print("[INFO] Playing audio...")
    if playsound:
        try:
            playsound(filepath)
            return
        except Exception as e:
            print(f"[WARN] playsound failed: {e}")

    # Fallback for Android intent
    os.system(f"am start -a android.intent.action.VIEW -d file://{filepath} -t audio/mpeg")

play_mp3(str(speech_file_path))
