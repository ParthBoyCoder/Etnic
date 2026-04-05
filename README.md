# Etnic

Etnic is a real-time voice AI assistant built using **ESP32 + INMP441 + WebSockets + Gemini API**.  
It captures audio from a microphone, streams it to a browser, processes it using AI, and returns a conversational response displayed on an OLED.

---

## ⚡ Features

- 🎤 Push-to-talk voice input (hardware button)
- ⚡ Real-time audio streaming via WebSockets
- 🧠 AI processing using Gemini (audio → text → response)
- 📺 OLED display with wrapped + scrolling text
- 🌐 Clean browser UI
- 🔁 Fully hardware-controlled interaction (no clicking required)

---

## 🧠 System Architecture

```text
ESP32 (Mic + Button + OLED)
↓ (WebSocket Audio Stream)
Browser (index2.html)
↓ (Base64 Audio)
Gemini API
↓ (Response)
Browser
↓ (WebSocket)
ESP32 OLED Display
```

---

## 🧰 Hardware Used

- ESP32 (WROOM)
- INMP441 I2S Microphone
- SSD1306 OLED Display (128x64)
- Push Button
- Jumper Wires

---

## 🔌 Wiring

### 🎤 INMP441 → ESP32

| INMP441 | ESP32 |
|--------|------|
| VCC | 3.3V |
| GND | GND |
| WS | GPIO 25 |
| SCK | GPIO 26 |
| SD | GPIO 33 |

---

### 📺 OLED (SSD1306 I2C)

| OLED | ESP32 |
|------|------|
| VCC | 3.3V |
| GND | GND |
| SCL | GPIO 23 |
| SDA | GPIO 21 |

---

### 🔘 Button

| Button | ESP32 |
|--------|------|
| One side | GPIO 4 |
| Other side | GND |

---

## 💻 Software Setup

### 1. Install Arduino IDE

Download from: [Arduino IDE](https://www.arduino.cc/en/software)

---

### 2. Install ESP32 Board

Go to **Preferences** and add this URL:

```text
https://dl.espressif.com/dl/package_esp32_index.json
```

---

### 3. Install Required Libraries

- WiFi
- WebSocketsServer
- Adafruit GFX
- Adafruit SSD1306
- ArduinoJson
- base64

---

### 4. Upload Code

Upload:

```text
sketch.ino
```

---

## 🌐 Web Interface Setup

Open:

```text
index2.html
```

---

## ⚠️ IMPORTANT: Update IP

In `index2.html`:

```js
const ESP_IP = "192.168.0.19";
```

Replace it with your ESP32 IP address from Serial Monitor.

Add your Gemini API key:

```js
const API_KEY = "YOUR_API_KEY";
```

Get it from: [Gemini API Key](https://makersuite.google.com/app/apikey)

---

## 🚀 How It Works

1. Press and hold the button 🎤.
2. Speak.
3. Release the button.
4. Audio is streamed to the browser.
5. Browser sends it to Gemini.
6. AI generates a response.
7. Response is sent back to the ESP32.
8. OLED displays the answer.

---

## 📺 OLED Output

- Auto word wrapping
- Vertical scrolling
- Clean multi-line display

---

## 🎥 Demo

```text
COMING SOON
```

---

## 🧪 Related Project

🎧 INMP441 Live Streaming Repo (by me):

https://github.com/ParthBoyCoder/INMP441

---

## ⚠️ Known Issues

- WebSocket may fail if ESP32 IP changes.
- Requires stable 2.4GHz WiFi.
- Audio quality depends on mic and environment.

---

## 🔮 Future Improvements

- 🔊 Speaker (text-to-speech)
- 🧠 Memory (context awareness)
- ⚡ Wake word detection
- 🌐 Cloud backend deployment
- 📦 Custom enclosure

---

## 💡 Inspiration

Built as a step toward creating a real hardware AI assistant system.

---

## 🏁 Final Note

This project combines:

- Embedded Systems
- Networking
- AI Integration

All in one system.
