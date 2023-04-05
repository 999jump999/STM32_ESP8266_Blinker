#define BLINKER_WIFI

#include <Blinker.h>

/* 配置ESP8266 */
char auth[] = "28e938a4367c"; //点灯科技设备KEY
char ssid[] = "Innovation laboratory";  //WIFI名称-只支持2.4G
char pswd[] = "99999999"; //WIFI密码
// char auth[] = "28e938a4367c"; //点灯科技设备KEY
// char ssid[] = "Mi10Pro";  //WIFI名称-只支持2.4G
// char pswd[] = "12345678"; //WIFI密码

/* 新建组件对象 */
BlinkerButton Button1("btn-a");//开关按键_客厅
BlinkerButton Button2("btn-b");//开关按键_餐厅
BlinkerButton Button3("btn-c");//开关按键_卧室
BlinkerButton Button4("btn-data");//开关按键_实时数据接收
BlinkerNumber Number1("num-temp");//数据_温度
BlinkerNumber Number2("num-humi");//数据_湿度
BlinkerSlider Slider1("ran-abc");//滑动条_电动窗帘

#define Rx_Buffer_Len      5
#define Tx_Buffer_Len      7         

int counter = 0;

int Real_time_data = 0;//定义数据实时传输
int temperature = 0;
int humidity = 0;

/*串口收发数据结构体*/
struct USART_DataStruct{		
  int Data[20]; //接收数据存储区
  int RxBuf;     //接收缓冲区
  int Count;
  int Flag;
} ;
struct USART_DataStruct Rx_Buffer;
// struct USART_DataStruct Tx_Buffer = {85,170,0,0,0,0,102};
struct USART_DataStruct Tx_Buffer = {0x55,0xAA,0x00,0x00,0x00,0x00,0x66};

/* 按下按键即会执行该函数 */
void button1_callback(const String & state)
{
  // BLINKER_LOG("get button1 state: ", state);
  if (state == BLINKER_CMD_ON) {
      // BLINKER_LOG("Toggle on!");
      digitalWrite(LED_BUILTIN, LOW);
      Button1.color("#0000FF");
      Button1.text("开客厅灯");
      Button1.print("on");
      Tx_Buffer.Data[2] = 1;
  }
  else if (state == BLINKER_CMD_OFF) {
      // BLINKER_LOG("Toggle off!");
      digitalWrite(LED_BUILTIN, HIGH);
      Button1.color("#808080");
      Button1.text("关客厅灯");
      Button1.print("off");
      Tx_Buffer.Data[2] = 0;
  }
  Write_STM32_Data(Tx_Buffer_Len);//发送给STM32数据包
}
void button2_callback(const String & state)
{
  // BLINKER_LOG("get button2 state: ", state);
  if (state == BLINKER_CMD_ON) {
    // BLINKER_LOG("Toggle on!");
    digitalWrite(LED_BUILTIN, LOW);
    Button2.color("#0000FF");
    Button2.text("开餐厅灯");
    Button2.print("on");
    Tx_Buffer.Data[3] = 1;
  }
  else if (state == BLINKER_CMD_OFF) {
    // BLINKER_LOG("Toggle off!");
    digitalWrite(LED_BUILTIN, HIGH);
    Button2.color("#808080");
    Button2.text("关餐厅灯");
    Button2.print("off");
    Tx_Buffer.Data[3] = 0;
  }
  Write_STM32_Data(Tx_Buffer_Len);//发送给STM32数据包
}
void button3_callback(const String & state)
{
  // BLINKER_LOG("get button3 state: ", state);
  if (state == BLINKER_CMD_ON) {
    // BLINKER_LOG("Toggle on!");
    digitalWrite(LED_BUILTIN, LOW);
    Button3.color("#0000FF");
    Button3.text("开卧室灯");
    Button3.print("on");
    Tx_Buffer.Data[4] = 1;
  }
  else if (state == BLINKER_CMD_OFF) {
    // BLINKER_LOG("Toggle off!");
    digitalWrite(LED_BUILTIN, HIGH);
    Button3.color("#808080");
    Button3.text("关卧室灯");
    Button3.print("off");
    Tx_Buffer.Data[4] = 0;
  }
  Write_STM32_Data(Tx_Buffer_Len);//发送给STM32数据包
}
void button4_callback(const String & state)
{
  if (state == BLINKER_CMD_ON) {
    // BLINKER_LOG("Real-time data reception : on!");
    Button4.icon("fas fa-toggle-on");
    Button4.text("开启数据实时传输");
    Button4.print("on");
    Real_time_data = 1;//开启数据实时传输
  }
  else if (state == BLINKER_CMD_OFF) {
    // BLINKER_LOG("Real-time data reception : off!");
    Button4.icon("fad fa-toggle-off");
    Button4.text("关闭数据实时传输");
    Button4.print("off");
    Real_time_data = 0;//关闭数据实时传输
  }
}

/* 滑动滑动条即会执行该函数 */
void slider1_callback(int32_t value)
{
  Tx_Buffer.Data[5] = value;
  Write_STM32_Data(Tx_Buffer_Len);//发送给STM32数据包
}

/* 如果未绑定的组件被触发，则会执行其中内容 */
void dataRead(const String & data)
{
  BLINKER_LOG("未绑定的组件被触发", data);
}

/* 心跳包函数 */
void heartbeat() {
  /* 反馈的内容 */
  Number1.print(temperature);
  Number2.print(humidity);
}

/* 历史数据存储功能，VIP可用，设定要存储的键名和值 */
// void dataStorage() {
//     Blinker.dataStorage("num-temp", temperature);
//     Blinker.dataStorage("num-humi", humidity);
// }

/* 接收STM32串口发送来的数据包 */
void Read_STM32_Data(void)       
{
  if(Serial.available() > 0) // 如果缓冲区中有数据，则读取并输出
  {
    Rx_Buffer.Data[Rx_Buffer.Count] = Serial.read();//读取传入或到达的数据字节
    Rx_Buffer.Count ++; //计数自增
    /*进行包头校验*/
    if(Rx_Buffer.Flag == 0)//置位接收开始标志
    {
      if(1 == Rx_Buffer.Count)
      {
        if(0xAA != Rx_Buffer.Data[0])//0xAA数据包头1
          Rx_Buffer.Count = 0;
      }
      else if(2 == Rx_Buffer.Count)
      {
        if(0xAA == Rx_Buffer.Data[1])//0xAA数据包头1
          Rx_Buffer.Count = 1;
        else if(0x55 != Rx_Buffer.Data[1])//0x55数据包头2
          Rx_Buffer.Count = 0;
      }
      else
      { /*进行包尾校验*/
        if(0x66 == Rx_Buffer.Data[Rx_Buffer_Len - 1])//0x66数据包尾
        {
          /*获取接收到的数据包中的重要信息*/
          temperature = Rx_Buffer.Data[2];//获取温度数据
          humidity = Rx_Buffer.Data[3];//获取湿度数据
          Rx_Buffer.Count = 0;//计数清零
          Rx_Buffer.Flag = 1;//置位接收完成标志          
        }
      }
    }         
  }
}

/* 发送ESP8266的数据包给STM32 */
void Write_STM32_Data(int Data_Len)
{
  for(int i = 0;i < Data_Len;i ++)
  {
    Serial.write(Tx_Buffer.Data[i]);
    // Serial.println(Tx_Buffer.Data[i]);
  }  
}

void setup()
{
  // 初始化串口
  Serial.begin(115200);
  BLINKER_DEBUG.stream(Serial);    
  // 初始化有LED的IO
  pinMode(LED_BUILTIN, OUTPUT);
  digitalWrite(LED_BUILTIN, HIGH);
  // 初始化blinker
  Blinker.begin(auth, ssid, pswd); 

  Blinker.attachData(dataRead); 
  Blinker.attachHeartbeat(heartbeat);  //注册一个心跳包
  //Blinker.attachDataStorage(dataStorage);   //开启历史数据存储功能，VIP可用
  /* 注册按键 */
  Button1.attach(button1_callback);
  Button2.attach(button2_callback);   
  Button3.attach(button3_callback);
  Button4.attach(button4_callback);

  Slider1.attach(slider1_callback);//注册滑块控件
}

void loop() {
  Blinker.run();

  Read_STM32_Data(); 
  if(Rx_Buffer.Flag == 1)//接收完成标志
  {
    Rx_Buffer.Flag = 0;//接收完成标志清零
    /*获取接收到的数据包中的重要信息*/
    temperature = Rx_Buffer.Data[2];//获取温度数据
    humidity = Rx_Buffer.Data[3];//获取湿度数据   
    // Serial.print("temperature:");
    // Serial.println(temperature);
    // Serial.print("humidity:"); 
    // Serial.println(humidity); 
    if(Real_time_data == 1) //开启数据实时传输，2S一次
    {
      Number1.print(temperature);
      Number2.print(humidity);
    }
    for(int i = 0;i < Rx_Buffer_Len;i ++)Rx_Buffer.Data[i] = 0;//清空数组，这很重要
  } 
}
