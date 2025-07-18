#include <driver/i2s.h>

// 🎤 MIC - INMP441 Pins (I2S0 for RX)
#define MIC_I2S_PORT I2S_NUM_0
#define MIC_BCLK     26  // SCK
#define MIC_WS       25  // WS (aka LRC)
#define MIC_SD       34  // SD (data out of mic)

// 🔊 SPEAKER - MAX98357A Pins (I2S1 for TX)
#define SPK_I2S_PORT I2S_NUM_1
#define SPK_BCLK     14  // BCLK
#define SPK_WS       15  // LRC
#define SPK_DIN      22  // DIN (data in of speaker)

#define SAMPLE_RATE      8000
#define SAMPLE_BITS      I2S_BITS_PER_SAMPLE_8BIT
#define I2S_READ_LEN     256
#define DMA_BUF_COUNT    4
#define DMA_BUF_LEN      256

void setupI2SMic() {
  i2s_config_t mic_config = {
    .mode = (i2s_mode_t)(I2S_MODE_MASTER | I2S_MODE_RX),
    .sample_rate = SAMPLE_RATE,
    .bits_per_sample = SAMPLE_BITS,
    .channel_format = I2S_CHANNEL_FMT_ONLY_LEFT,
    .communication_format = I2S_COMM_FORMAT_I2S,
    .intr_alloc_flags = 0,
    .dma_buf_count = DMA_BUF_COUNT,
    .dma_buf_len = DMA_BUF_LEN,
    .use_apll = false,
    .tx_desc_auto_clear = false,
    .fixed_mclk = 0
  };

  i2s_pin_config_t mic_pins = {
    .bck_io_num = MIC_BCLK,
    .ws_io_num = MIC_WS,
    .data_in_num = MIC_SD,
    .data_out_num = I2S_PIN_NO_CHANGE
  };

  i2s_driver_install(MIC_I2S_PORT, &mic_config, 0, NULL);
  i2s_set_pin(MIC_I2S_PORT, &mic_pins);
  i2s_zero_dma_buffer(MIC_I2S_PORT);
}

void setupI2SSpeaker() {
  i2s_config_t spk_config = {
    .mode = (i2s_mode_t)(I2S_MODE_MASTER | I2S_MODE_TX),
    .sample_rate = SAMPLE_RATE,
    .bits_per_sample = SAMPLE_BITS,
    .channel_format = I2S_CHANNEL_FMT_ONLY_LEFT,
    .communication_format = I2S_COMM_FORMAT_I2S,
    .intr_alloc_flags = 0,
    .dma_buf_count = DMA_BUF_COUNT,
    .dma_buf_len = DMA_BUF_LEN,
    .use_apll = false,
    .tx_desc_auto_clear = true,
    .fixed_mclk = 0
  };

  i2s_pin_config_t spk_pins = {
    .bck_io_num = SPK_BCLK,
    .ws_io_num = SPK_WS,
    .data_out_num = SPK_DIN,
    .data_in_num = I2S_PIN_NO_CHANGE
  };

  i2s_driver_install(SPK_I2S_PORT, &spk_config, 0, NULL);
  i2s_set_pin(SPK_I2S_PORT, &spk_pins);
  i2s_zero_dma_buffer(SPK_I2S_PORT);
}

void setup() {
  Serial.begin(115200);
  delay(1000);
  Serial.println("🎤🔁🔊 Starting Loopback 8kHz 8-bit Test...");

  setupI2SMic();
  setupI2SSpeaker();
}

void loop() {
  uint8_t buf[I2S_READ_LEN];
  size_t bytesRead, bytesWritten;

  i2s_read(MIC_I2S_PORT, &buf, I2S_READ_LEN, &bytesRead, portMAX_DELAY);
  i2s_write(SPK_I2S_PORT, &buf, bytesRead, &bytesWritten, portMAX_DELAY);
}
