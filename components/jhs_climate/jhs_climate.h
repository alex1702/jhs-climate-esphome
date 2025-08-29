class JHSClimate {
public:
  JHSClimate(int panel_tx_pin, int panel_rx_pin, int uart_num = UART_NUM1);

  void setup();
  void loop();

private:
  int tx_pin_;
  int rx_pin_;
  uart_port_t uart_num_;
};
