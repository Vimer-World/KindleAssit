#include <libusb-1.0/libusb.h>
#include <stdbool.h>
#include <stdio.h>

/* A usb device can be identified uniqly given vid and pid
 * 1949  Lab126, Inc.
 *         0002  Amazon Kindle
 * 	       0004  Amazon Kindle 3/4/Paperwhite
 * 	       0006  Kindle Fire
 * 	       0008  Amazon Kindle Fire HD 8.9"
 * http://www.linux-usb.org/usb.ids
 */

bool is_kindle_connected(libusb_device *dev) {
	static unsigned int VENDOR = 0x1949;
	enum kindle {
		Kindle           = 0x2,
		KindlePaperwhite = 0x4,
		KindleFire       = 0x6,
		KindleFireHD     = 0x8,
	};

	enum libusb_error err;

	struct libusb_device_descriptor desc;
	err = libusb_get_device_descriptor(dev, &desc);
	if (err != LIBUSB_SUCCESS) {
		printf("Error: %s\n", libusb_strerror(err));
	}

#if 0
	libusb_device_handle *handle;
	err = libusb_open(dev, &handle);
	if (err != LIBUSB_SUCCESS) {
		printf("Error: %s\n", libusb_strerror(err));
	}

	/* This actually returns an OS String. I don't know how to return a UTF-8
	 * string, or a unicode representation without konwing how to set the
	 * `langid'.
	 */
	unsigned char product_name[128];
	unsigned char manufacturer[128];
	libusb_get_string_descriptor_ascii(handle,
			desc.iManufacturer, manufacturer, sizeof(manufacturer));
	libusb_get_string_descriptor_ascii(handle,
			desc.iProduct, product_name, sizeof(product_name));
	printf("vendor_id: 0x%04x, product_id: 0x%04x\n", desc.idVendor, desc.idProduct);
	printf("manufacturer: %s, deivice: %s\n", manufacturer, product_name);

	libusb_close(handle);
#endif

	return desc.idVendor == VENDOR && (
		   desc.idProduct == Kindle
		|| desc.idProduct == KindleFire
		|| desc.idProduct == KindlePaperwhite
		|| desc.idProduct == KindleFireHD );
}

int main(int argc, char *argv[]) {
	enum libusb_error err;

	libusb_context *ctx;
	err	= libusb_init(&ctx);
	if (err < 0) printf("Error: %s\n", libusb_strerror(err));

	libusb_device **list;
	ssize_t cnt = libusb_get_device_list(ctx, &list);
	if (cnt < 0) printf("Error: %s\n", libusb_strerror(cnt));

	for (int i = 0; i < cnt; ++i) {
		is_kindle_connected(list[i]);
	}

	libusb_free_device_list(list, true);
	libusb_exit(ctx);
	return 0;
}
