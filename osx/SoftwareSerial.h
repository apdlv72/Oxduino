

class SoftwareSerial : public HardwareSerial
{
  public:
    SoftwareSerial(int rxPin, int txPin) : HardwareSerial()
    {
    }
    
    void flush()
    {
    };
};

