from flask import Flask, request, jsonify
from groq import Groq
import os

app = Flask(__name__)

# Set your Groq API key here
os.environ["GROQ_API_KEY"] = "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"
client = Groq()

@app.route('/groq', methods=['POST'])
def get_response():
    user_input = request.json.get("message")
    
    completion = client.chat.completions.create(
        model="meta-llama/llama-4-maverick-17b-128e-instruct",
        messages=[{"role": "user", "content": user_input}],
        temperature=1.3,
        max_completion_tokens=512,
        top_p=0.95,
        stream=False
    )
    
    return jsonify({"response": completion.choices[0].message.content})

if __name__ == '__main__':
    app.run(host='0.0.0.0', port=5000)
