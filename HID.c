#include "HID.h"

USB_JoystickReport_Input_t next_report;

// Reset report to default.
void ResetReport(void)
{
  memset(&next_report, 0, sizeof(USB_JoystickReport_Input_t));
  next_report.LX = STICK_CENTER;
  next_report.LY = STICK_CENTER;
  next_report.RX = STICK_CENTER;
  next_report.RY = STICK_CENTER;
  next_report.HAT = HAT_CENTER;
}

void SetButtons(const uint16_t Button) 
{
  next_report.Button = Button;
}

void PressButtons(const uint16_t Button) 
{
  next_report.Button |= Button;
}

void ReleaseButtons(const uint16_t Button) 
{
  next_report.Button &= ~(Button);
}

void SetHATSwitch(const uint8_t HAT) 
{
  next_report.HAT = HAT;
}

void SetLeftStick(const uint8_t LX, const uint8_t LY)
{
  next_report.LX = LX; 
  next_report.LY = LY;
}

void SetRightStick(const uint8_t RX, const uint8_t RY)
{
  next_report.RX = RX; 
  next_report.RY = RY;
}

void HIDInit(void)
{
  ResetReport();
  //USB_Init();
}

// Prepare the next report for the host.
inline void GetNextReport(USB_JoystickReport_Input_t *const ReportData)
{
	memcpy(ReportData, &next_report, sizeof(USB_JoystickReport_Input_t));
}

// Process and deliver data from IN and OUT endpoints.
void Report_Task(void)
{
  // If the device isn't connected and properly configured, we can't do anything here.
  if (USB_DeviceState != DEVICE_STATE_Configured)
    return;

  // [Optimized] Only send data when changed.
  if (echo_ms == 0)
  {
    // We'll create an empty report.
    USB_JoystickReport_Input_t JoystickInputData;
    // We'll then populate this report with what we want to send to the host.
    GetNextReport(&JoystickInputData);
    // Once populated, we can output this data to the host. We do this by first writing the data to the control stream.

    Echo_Report();
    // set interval
    echo_ms = ECHO_INTERVAL;
  }
}

void HIDTask(void)
{
  // We need to run our task to process and deliver data for our IN and OUT endpoints.
  Report_Task();
  // We also need to run the main USB management task.
  USB_USBTask();
}

// Fired to indicate that the device is enumerating.
void EVENT_USB_Device_Connect(void)
{
  // We can indicate that we're enumerating here (via status LEDs, sound, etc.).

  Device_Connected();
}

// Fired to indicate that the device is no longer connected to a host.
void EVENT_USB_Device_Disconnect(void)
{
  // We can indicate that our device is not ready (via status LEDs, sound, etc.).
}


