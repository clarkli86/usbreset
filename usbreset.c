/* usbreset -- send a USB port reset to a USB device */

#include <stdio.h>
#include <unistd.h>
#include <fcntl.h>
#include <errno.h>
#include <libusb-1.0/libusb.h>
#include <linux/usbdevice_fs.h>


int main(int argc, char **argv)
{
    int fd, ret, vid, pid;

    if (argc != 3) {
      fprintf(stderr, "Usage: usbreset vid pid\n");
      return 1;
    }
    sscanf(argv[1], "%x", &vid);
    sscanf(argv[2], "%x", &pid);

    printf("Looking for %x/%x\n", vid, pid);

    libusb_init(NULL);
    // discover devices
    libusb_device **list;
    ssize_t cnt = libusb_get_device_list(NULL, &list);
    ssize_t i = 0;
    int err = 0;
    if (cnt < 0) {
      printf("Failed to get usb device list\n");
      return -1;
    }
    for (i = 0; i < cnt; i++) {
      libusb_device *device = list[i];
      libusb_device_descriptor desc = {0};
      err = libusb_get_device_descriptor(device, &desc);
      if(err) {
        printf("Failed to get device descriptor\n");
        goto Error;
      } 
      if (desc.idVendor == vid && desc.idProduct == pid) {
        libusb_device_handle *handle;
        err = libusb_open(device, &handle);
        if (err) {
          printf("Failed to open device\n");
          goto Error;
        }
        if(!libusb_reset_device(handle))
          printf("Succesfully reset device at bus %d\n", libusb_get_bus_number(device));
        libusb_close(handle);
      }
    }
Error:
  libusb_free_device_list(list, 1);
  return 0;
}
