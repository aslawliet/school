from groq import Groq
import serial
import time

# Initialize Groq client
client = Groq(api_key="gsk_z2s211CyyGAzP3MyMwH2WGdyb3FYMC7abn7o6p0aNFNrTKv7     # HXy5")

while True:
    # User input for AI
    user_input = input("You: ")

    # Call the Groq API
    completion = client.chat.completions.create(
        model="meta-llama/llama-4-maverick-17b-128e-instruct",
        messages=[{"role": "user", "content": user_input}],
        temperature=1.3,
        max_completion_tokens=100,
        top_p=0.95,
        stream=False
    )

    # Extract response
    response = completion.choices[0].message.content.strip()

    # Show in terminal
    print("AI:", response)

    # Send to Arduino via Serial (adjust COM port as needed)
    try:
        ser = serial.Serial('COM7', 9600, timeout=1)  # Replace COM6 with your actual port
        time.sleep(2)  # Wait for Arduino to reset

        # Send line by line if longer than 32 chars
        for line in [response[i:i+32] for i in range(0, len(response), 32)]:
            ser.write((line + "\n").encode('utf-8'))
            time.sleep(1)  # Give time to display

        ser.close()
    except Exception as e:
        print("Error sending to Arduino:", e)
