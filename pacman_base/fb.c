#include "bcm.h"
#include "mailbox.h"
#include "fb.h"

#define BITS_PER_BYTE 8

typedef struct {
  unsigned int width;       // width of the display
  unsigned int height;      // height of the display
  unsigned int virtual_width;  // width of the virtual framebuffer
  unsigned int virtual_height; // height of the virtual framebuffer
  unsigned int pitch;       // number of bytes per row
  unsigned int depth;       // number of bits per pixel
  unsigned int x_offset;    // x of the upper left corner of the virtual fb
  unsigned int y_offset;    // y of the upper left corner of the virtual fb
  unsigned int framebuffer; // pointer to the start of the framebuffer
  unsigned int size;        // number of bytes in the framebuffer
} fb_config_t;

// fb is volatile because the GPU will write to it
static volatile fb_config_t fb __attribute__ ((aligned(16)));

// depth is in bytes
void fb_init(unsigned width, unsigned height, unsigned depth, unsigned db) {
  fb.width = width;
  fb.virtual_width = width;
  fb.height = height;
  fb.virtual_height = db ? height*2 : height; // double height if double buffering
  fb.depth = depth * BITS_PER_BYTE; // convert number of bytes to number of bits
  fb.x_offset = 0;
  fb.y_offset = 0;

  // set values returned by the GPU to 0; see mailbox manual
  fb.pitch = 0;
  fb.framebuffer = 0;
  fb.size = 0;

  mailbox_write(MAILBOX_FRAMEBUFFER, (unsigned)&fb + GPU_NOCACHE);
  // Need to read so mailbox gets set to empty again (so system doesn't hang)
  (void) mailbox_read(MAILBOX_FRAMEBUFFER);
}

/*
 * Swaps the front buffer (displayed) and the back buffer (drawn into). To do so, toggles
 * the y_offset between 0 and physical `height` to go between the two stacked buffers.
 */
void fb_swap_buffer(void) {
  // Only allow buffer swap if doublebuffer is on
  if (fb.virtual_height == fb.height) return;

  if (fb.y_offset == 0) {
    fb.y_offset = fb.height;
  } else {
    fb.y_offset = 0;
  }

  mailbox_write(MAILBOX_FRAMEBUFFER, (unsigned)&fb + GPU_NOCACHE);
  (void) mailbox_read(MAILBOX_FRAMEBUFFER);
}


unsigned char* fb_get_draw_buffer(void) {
  // If single buffer or front buffer starts at physical `height`
  if (fb.virtual_height == fb.height || fb.y_offset == fb.height)
    return (unsigned char*) fb.framebuffer;

  // If front buffer starts at 0
  return (unsigned char*) fb.framebuffer + (fb.pitch * fb.height);
}

// =========== GETTER FUNCTIONS =========== //
unsigned fb_get_width(void) {
  return fb.width;
}

unsigned fb_get_height(void) {
  return fb.height;
}

unsigned fb_get_depth(void) {
  return fb.depth;
}

unsigned fb_get_pitch(void) {
  return fb.pitch;
}


