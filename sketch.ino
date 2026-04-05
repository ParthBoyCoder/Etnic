#include <WiFi.h>
#include <WebSocketsServer.h>
#include <driver/i2s.h>
#include <Wire.h>
#include <Adafruit_GFX.h>
#include <Adafruit_SSD1306.h>

// ===== WIFI =====
#define WIFI_SSID "UR WIFI SSID"
#define WIFI_PASS "UR WIFI PASS"

// ===== PINS =====
#define I2S_BCLK 26
#define I2S_LRCL 25
#define I2S_DIN  33
#define BUTTON   4
#define OLED_SDA 21
#define OLED_SCL 23

#define SAMPLE_RATE 16000

WebSocketsServer wsServer(81);
Adafruit_SSD1306 oled(128, 64, &Wire, -1);

// ===== OLED =====
void showMsg(String a, String b="") {
  oled.clearDisplay();
  oled.setTextSize(1);
  oled.setTextColor(WHITE);
  oled.setCursor(0,0); oled.println(a);
  oled.setCursor(0,20); oled.println(b);
  oled.display();
}

void showScrolling(String text) {

  const int maxCharsPerLine = 21;
  const int maxLines = 4;

  // ===== LINE BUFFER =====
  String lines[50];
  int totalLines = 0;

  String current = "";

  // ===== WORD WRAP =====
  for (int i = 0; i < text.length(); i++) {
    char c = text[i];

    current += c;

    // wrap if line full
    if (current.length() >= maxCharsPerLine || c == '\n') {
      lines[totalLines++] = current;
      current = "";
    }
  }

  // last line
  if (current.length() > 0) {
    lines[totalLines++] = current;
  }

  // ===== DISPLAY WITH TRUE VERTICAL SCROLL =====
  int start = 0;

  while (true) {

    oled.clearDisplay();
    oled.setCursor(0, 0);

    // show only last 4 lines window
    for (int i = 0; i < maxLines; i++) {
      int idx = start + i;
      if (idx < totalLines) {
        oled.println(lines[idx]);
      }
    }

    oled.display();

    // stop if all lines shown
    if (start + maxLines >= totalLines) break;

    start++;
    delay(1200);  // scroll speed
  }
}

// ===== I2S =====
void setupI2S() {
  i2s_config_t config = {
    .mode = (i2s_mode_t)(I2S_MODE_MASTER | I2S_MODE_RX),
    .sample_rate = SAMPLE_RATE,
    .bits_per_sample = I2S_BITS_PER_SAMPLE_32BIT,
    .channel_format = I2S_CHANNEL_FMT_ONLY_LEFT,
    .communication_format = I2S_COMM_FORMAT_I2S,
    .intr_alloc_flags = ESP_INTR_FLAG_LEVEL1,
    .dma_buf_count = 8,
    .dma_buf_len = 1024,
    .use_apll = false
  };

  i2s_pin_config_t pins = {
    .bck_io_num = I2S_BCLK,
    .ws_io_num = I2S_LRCL,
    .data_out_num = -1,
    .data_in_num = I2S_DIN
  };

  i2s_driver_install(I2S_NUM_0, &config, 0, NULL);
  i2s_set_pin(I2S_NUM_0, &pins);
  i2s_zero_dma_buffer(I2S_NUM_0);
}

// ===== RECEIVE AI =====
void onWebSocketEvent(uint8_t num, WStype_t type, uint8_t * payload, size_t length) {
  if (type == WStype_TEXT) {
    String msg = String((char*)payload);
    Serial.println("AI: " + msg);
    showScrolling(msg);
  }
}

// ===== SETUP =====
void setup() {
  Serial.begin(115200);

  pinMode(BUTTON, INPUT_PULLUP);

  Wire.begin(OLED_SDA, OLED_SCL);
  oled.begin(SSD1306_SWITCHCAPVCC, 0x3C);

  showMsg("WiFi...");
  WiFi.begin(WIFI_SSID, WIFI_PASS);
  while (WiFi.status() != WL_CONNECTED) delay(200);

  showMsg("Connected!", WiFi.localIP().toString());

  wsServer.begin();
  wsServer.onEvent(onWebSocketEvent);

  setupI2S();
}

// ===== LOOP =====
void loop() {
  wsServer.loop();

  static bool recording = false;
  bool pressed = digitalRead(BUTTON) == LOW;

  // START
  if (pressed && !recording) {
    recording = true;
    wsServer.broadcastTXT("START");
    showMsg("REC ●", "Speak...");
    delay(200);
  }

  // STOP
  if (!pressed && recording) {
    recording = false;
    wsServer.broadcastTXT("STOP");
    showMsg("Thinking...");
  }

  // STREAM AUDIO
  if (recording) {
    const int samples = 512;
    int32_t buffer32[samples];
    int16_t buffer16[samples];

    size_t bytesRead;
    i2s_read(I2S_NUM_0, buffer32, sizeof(buffer32), &bytesRead, portMAX_DELAY);

    for (int i = 0; i < samples; i++) {
      buffer16[i] = buffer32[i] >> 16; // 🔥 proven working
    }

    wsServer.broadcastBIN((uint8_t*)buffer16, samples * 2);
  }

  delay(5);
}