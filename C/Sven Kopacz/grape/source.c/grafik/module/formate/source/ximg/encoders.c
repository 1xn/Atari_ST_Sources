#include "imgcodec.h"

/* Local extension of public image buffer struct for encoding. */

typedef struct ibuf
{
  IBUFPUB pub;
  char *line_adr;
  long length;
  long byte_width;	/* Only used in Level-1 encoding. */
  char *image_adr;
  char *image_ptr;	/* Only used in Level-3 encoding. */
  void (*raw_encode)(struct ibuf *ip);
  void (*user_exit)(void);
  FBUFPUB *output;
  short out_vrc;
  short height;
  char data[0];
}
IBUF;

/* Forward declarations of local functions. */

static void level_3_putline(IBUF *ip);
static void level_2_putline(IBUF *ip);
static void level_1_putline(IBUF *ip);

static void l_3_p_1_encode(IBUF *ip);
static void l_3_p_2_encode(IBUF *ip);
static void l_2_p_1_encode(IBUF *ip);
static void l_2_p_2_encode(IBUF *ip);
static void l_1_p_1_encode(IBUF *ip);
static void l_1_p_2_encode(IBUF *ip);

/* Generic initialization function for encoding.
 * Causes all defined encoding functions to be linked to the caller.
 * Returns a zero pointer if user_malloc failes.
 * Otherwise the returned pointer has to be freed after
 * processing by the 'complement' of user_malloc.
 */

IBUFPUB *encode_init(IMG_HEADER *input_header, FBUFPUB *output,
		     void *(*user_malloc)(long size),
		     void (*user_exit)(void),
		     short out_lev, short out_pat)
{
  long byte_width, line_len, buf_len;
  IBUF *image;

  byte_width = (input_header->sl_width + 7) >> 3;
  line_len = byte_width * input_header->planes;
  /* Alloc double line buffer for out_vrc handling (one line delay) */
  buf_len = line_len << 1;
  /* For Level-2 we need an additional line buffer for extended vrc
   * handling.
   */
  if (out_lev == 2) buf_len += line_len;
  else if (out_lev != 1)
    /* Level-3 encoder requires a full image buffer. */
    buf_len += line_len * input_header->sl_height;
  image =
    (*user_malloc)(sizeof(IBUF) + buf_len + input_header->pat_run);
  if (image)
  {
    image->line_adr = image->pub.pbuf = image->data;
    image->image_adr = image->data + (line_len << 1);
    image->pub.pat_buf = image->data + buf_len;
    image->pub.bytes_left = line_len;
    image->length = line_len;
    image->byte_width = byte_width;
    image->pub.pat_run = input_header->pat_run;
    image->height = input_header->sl_height;
    image->pub.vrc = 1; image->pub.init = 0;
    image->output = output; image->user_exit = user_exit;

    image->pub.put_line = (void (*)(IBUFPUB *))
      (out_lev == 1 ? level_1_putline :
       out_lev == 2 ? level_2_putline : level_3_putline);

    image->raw_encode =
	 out_lev == 1
      ? (out_pat == 1 ? l_1_p_1_encode : l_1_p_2_encode)
      :  out_lev == 2
      ? (out_pat == 1 ? l_2_p_1_encode : l_2_p_2_encode)
      : (out_pat == 1 ? l_3_p_1_encode : l_3_p_2_encode);
  }
  return &image->pub;
}

/* Here are the special encode initialization functions.
 * The reason for introducing them is, that an application usually
 * needs only one encoder. The programmer has the freedom
 * (or pain ;-) to decide. For instance, a snapshot utility is
 * recommended to use a Level-2 encoder, because extended vrc
 * may provide significant savings over Level-1, and Level-3 could
 * rather fail in memory allocation (if Level-3 is nevertheless
 * desired, there should be at least an alternative Level-2 mode
 * built in to be used in case of Level-3 failing).
 */

IBUFPUB *l3p2_encode_init(IMG_HEADER *input_header, FBUFPUB *output,
			  void *(*user_malloc)(long size),
			  void (*user_exit)(void))
{
  long byte_width, line_len, buf_len;
  IBUF *image;

  byte_width = (input_header->sl_width + 7) >> 3;
  line_len = byte_width * input_header->planes;
  /* Alloc double line buffer for out_vrc handling (one line delay) */
  buf_len = line_len << 1;
  /* Level-3 encoder requires a full image buffer. */
  buf_len += line_len * input_header->sl_height;
  image =
    (*user_malloc)(sizeof(IBUF) + buf_len + input_header->pat_run);
  if (image)
  {
    image->line_adr = image->pub.pbuf = image->data;
    image->image_adr = image->data + (line_len << 1);
    image->pub.pat_buf = image->data + buf_len;
    image->pub.bytes_left = line_len;
    image->length = line_len;
    image->pub.pat_run = input_header->pat_run;
    image->height = input_header->sl_height;
    image->pub.vrc = 1; image->pub.init = 0;
    image->output = output; image->user_exit = user_exit;
    image->pub.put_line = (void (*)(IBUFPUB *))level_3_putline;
    image->raw_encode = l_3_p_2_encode;
  }
  return &image->pub;
}

IBUFPUB *l3p1_encode_init(IMG_HEADER *input_header, FBUFPUB *output,
			  void *(*user_malloc)(long size),
			  void (*user_exit)(void))
{
  long byte_width, line_len, buf_len;
  IBUF *image;

  byte_width = (input_header->sl_width + 7) >> 3;
  line_len = byte_width * input_header->planes;
  /* Alloc double line buffer for out_vrc handling (one line delay) */
  buf_len = line_len << 1;
  /* Level-3 encoder requires a full image buffer. */
  buf_len += line_len * input_header->sl_height;
  image =
    (*user_malloc)(sizeof(IBUF) + buf_len + input_header->pat_run);
  if (image)
  {
    image->line_adr = image->pub.pbuf = image->data;
    image->image_adr = image->data + (line_len << 1);
    image->pub.pat_buf = image->data + buf_len;
    image->pub.bytes_left = line_len;
    image->length = line_len;
    image->pub.pat_run = input_header->pat_run;
    image->height = input_header->sl_height;
    image->pub.vrc = 1; image->pub.init = 0;
    image->output = output; image->user_exit = user_exit;
    image->pub.put_line = (void (*)(IBUFPUB *))level_3_putline;
    image->raw_encode = l_3_p_1_encode;
  }
  return &image->pub;
}

IBUFPUB *l2p2_encode_init(IMG_HEADER *input_header, FBUFPUB *output,
			  void *(*user_malloc)(long size),
			  void (*user_exit)(void))
{
  long byte_width, line_len, buf_len;
  IBUF *image;

  byte_width = (input_header->sl_width + 7) >> 3;
  line_len = byte_width * input_header->planes;
  /* Alloc double line buffer for out_vrc handling (one line delay) */
  buf_len = line_len << 1;
  /* For Level-2 we need an additional line buffer for extended vrc
   * handling.
   */
  buf_len += line_len;
  image =
    (*user_malloc)(sizeof(IBUF) + buf_len + input_header->pat_run);
  if (image)
  {
    image->line_adr = image->pub.pbuf = image->data;
    image->image_adr = image->data + (line_len << 1);
    image->pub.pat_buf = image->data + buf_len;
    image->pub.bytes_left = line_len;
    image->length = line_len;
    image->pub.pat_run = input_header->pat_run;
    image->height = input_header->sl_height;
    image->pub.vrc = 1; image->pub.init = 0;
    image->output = output; image->user_exit = user_exit;
    image->pub.put_line = (void (*)(IBUFPUB *))level_2_putline;
    image->raw_encode = l_2_p_2_encode;
  }
  return &image->pub;
}

IBUFPUB *l2p1_encode_init(IMG_HEADER *input_header, FBUFPUB *output,
			  void *(*user_malloc)(long size),
			  void (*user_exit)(void))
{
  long byte_width, line_len, buf_len;
  IBUF *image;

  byte_width = (input_header->sl_width + 7) >> 3;
  line_len = byte_width * input_header->planes;
  /* Alloc double line buffer for out_vrc handling (one line delay) */
  buf_len = line_len << 1;
  /* For Level-2 we need an additional line buffer for extended vrc
   * handling.
   */
  buf_len += line_len;
  image =
    (*user_malloc)(sizeof(IBUF) + buf_len + input_header->pat_run);
  if (image)
  {
    image->line_adr = image->pub.pbuf = image->data;
    image->image_adr = image->data + (line_len << 1);
    image->pub.pat_buf = image->data + buf_len;
    image->pub.bytes_left = line_len;
    image->length = line_len;
    image->pub.pat_run = input_header->pat_run;
    image->height = input_header->sl_height;
    image->pub.vrc = 1; image->pub.init = 0;
    image->output = output; image->user_exit = user_exit;
    image->pub.put_line = (void (*)(IBUFPUB *))level_2_putline;
    image->raw_encode = l_2_p_1_encode;
  }
  return &image->pub;
}

IBUFPUB *l1p2_encode_init(IMG_HEADER *input_header, FBUFPUB *output,
			  void *(*user_malloc)(long size),
			  void (*user_exit)(void))
{
  long byte_width, line_len, buf_len;
  IBUF *image;

  byte_width = (input_header->sl_width + 7) >> 3;
  line_len = byte_width * input_header->planes;
  /* Alloc double line buffer for out_vrc handling (one line delay) */
  buf_len = line_len << 1;
  image =
    (*user_malloc)(sizeof(IBUF) + buf_len + input_header->pat_run);
  if (image)
  {
    image->line_adr = image->pub.pbuf = image->data;
    image->image_adr = image->pub.pat_buf = image->data + buf_len;
    image->pub.bytes_left = line_len;
    image->length = line_len;
    image->byte_width = byte_width;
    image->pub.pat_run = input_header->pat_run;
    image->height = input_header->sl_height;
    image->pub.vrc = 1; image->pub.init = 0;
    image->output = output; image->user_exit = user_exit;
    image->pub.put_line = (void (*)(IBUFPUB *))level_1_putline;
    image->raw_encode = l_1_p_2_encode;
  }
  return &image->pub;
}

IBUFPUB *l1p1_encode_init(IMG_HEADER *input_header, FBUFPUB *output,
			  void *(*user_malloc)(long size),
			  void (*user_exit)(void))
{
  long byte_width, line_len, buf_len;
  IBUF *image;

  byte_width = (input_header->sl_width + 7) >> 3;
  line_len = byte_width * input_header->planes;
  /* Alloc double line buffer for out_vrc handling (one line delay) */
  buf_len = line_len << 1;
  image =
    (*user_malloc)(sizeof(IBUF) + buf_len + input_header->pat_run);
  if (image)
  {
    image->line_adr = image->pub.pbuf = image->data;
    image->image_adr = image->pub.pat_buf = image->data + buf_len;
    image->pub.bytes_left = line_len;
    image->length = line_len;
    image->byte_width = byte_width;
    image->pub.pat_run = input_header->pat_run;
    image->height = input_header->sl_height;
    image->pub.vrc = 1; image->pub.init = 0;
    image->output = output; image->user_exit = user_exit;
    image->pub.put_line = (void (*)(IBUFPUB *))level_1_putline;
    image->raw_encode = l_1_p_1_encode;
  }
  return &image->pub;
}

/* Here come the local functions for encode processing. */

static void l_3_p_2_encode(IBUF *ip)
{
  char cdata, c_val, *line_buf, *line_ptr, *endl_buf;
  long s_save, p_save, v_save, buf_offs;
  char *srp, *prp, *vrp;
  FBUFPUB *output;

  endl_buf = ip->image_ptr; buf_offs = -ip->length;
  line_ptr = line_buf = ip->image_adr;
  output = ip->output;
  do
  { /* Level-3 output loop equals Level-2
     * (different initializations).
     * So look at the below Level-2 code for comments.
     */
    char *line_lim0 = line_buf + 256;
    if (line_lim0 > endl_buf) line_lim0 = endl_buf;
    do
    { char *start_ptr = line_buf;
      s_save = -1;
      if ((cdata = *line_buf) == 0 || cdata == -1)
      {
	char *line_lim = line_buf + 127;
	while (line_buf < endl_buf)
	{
	  if (*line_buf != cdata)
	  {
	    cdata = ~cdata;
	    if (*line_buf != cdata) break;
	    line_lim = line_buf;
	  }
	  if (line_buf >= line_lim)
	  {
	    s_save -= 1; line_lim += 127;
	  }
	  line_buf++;
	}
	cdata = *start_ptr;
      }
      srp = line_buf; line_buf = start_ptr;
      s_save += srp - line_buf;
      if (line_buf != line_ptr && srp < line_lim0) s_save -= 2;
      if (line_buf < endl_buf - 1)
      {
	char *line_lim = line_buf + 255 * 2;
	if (line_lim > endl_buf) line_lim = endl_buf;
	--line_lim;
	c_val = line_buf[1];
	while (line_buf < line_lim &&
	       line_buf[0] == cdata &&
	       line_buf[1] == c_val) line_buf += 2;
      }
      prp = line_buf; line_buf = start_ptr;
      p_save = (prp - line_buf) - 4;
      if (line_buf != line_ptr && prp < line_lim0) p_save -= 2;
      v_save = -3;
      {
	char *line_lim = line_buf + 255;
	while (line_buf < endl_buf &&
	       line_buf[0] == line_buf[buf_offs])
	{
	  if (line_buf >= line_lim)
	  {
	    v_save -= 3; line_lim += 255;
	  }
	  line_buf++;
      } }
      vrp = line_buf; line_buf = start_ptr;
      v_save += vrp - line_buf;
      if (line_buf != line_ptr && vrp < line_lim0) v_save -= 2;
    }
    while (s_save < 0 && p_save < 0 && v_save < 0 &&
	   ++line_buf < line_lim0);
    if (line_buf != line_ptr)
    {
      FPUTC(output, 0x80);
      FPUTC(output, (char)(line_buf - line_ptr));
      do FPUTC(output, *line_ptr++);
      while (line_ptr < line_buf);
    }
    if (s_save >= 0 && s_save >= p_save && s_save >= v_save)
    {
      char *line_lim = line_buf + 127;
      if (line_lim > endl_buf) line_lim = endl_buf;
      while (line_buf < line_lim && *line_buf == cdata) line_buf++;
      cdata <<= 7;
      FPUTC(output, cdata | (char)(line_buf - line_ptr));
      line_ptr = line_buf;
    }
    else if (p_save >= 0 && p_save >= v_save)
    {
      FPUTC(output, 0);
      FPUTC(output, (char)((prp - line_buf) >> 1));
      FPUTC(output, cdata);
      FPUTC(output, c_val);
      line_ptr = line_buf = prp;
    }
    else if (v_save >= 0)
    {
      line_buf += 255;
      if (line_buf > vrp) line_buf = vrp;
      FPUTC(output, 0);
      FPUTC(output, 0);
      FPUTC(output, (char)(line_buf - line_ptr) - 1);
      line_ptr = line_buf;
  } }
  while (line_buf < endl_buf);

  ip->image_adr = ip->image_ptr;
}

static void l_3_p_1_encode(IBUF *ip)
{
  char cdata, *line_buf, *line_ptr, *endl_buf;
  long s_save, p_save, v_save, buf_offs;
  char *srp, *prp, *vrp;
  FBUFPUB *output;

  endl_buf = ip->image_ptr; buf_offs = -ip->length;
  line_ptr = line_buf = ip->image_adr;
  output = ip->output;
  do
  { /* Level-3 output loop equals Level-2
     * (different initializations).
     * So look at the below Level-2 code for comments.
     */
    char *line_lim0 = line_buf + 256;
    if (line_lim0 > endl_buf) line_lim0 = endl_buf;
    do
    { char *start_ptr = line_buf;
      s_save = -1;
      if ((cdata = *line_buf) == 0 || cdata == -1)
      {
	char *line_lim = line_buf + 127;
	while (line_buf < endl_buf)
	{
	  if (*line_buf != cdata)
	  {
	    cdata = ~cdata;
	    if (*line_buf != cdata) break;
	    line_lim = line_buf;
	  }
	  if (line_buf >= line_lim)
	  {
	    s_save -= 1; line_lim += 127;
	  }
	  line_buf++;
	}
	cdata = *start_ptr;
      }
      srp = line_buf; line_buf = start_ptr;
      s_save += srp - line_buf;
      if (line_buf != line_ptr && srp < line_lim0) s_save -= 2;
      {
	char *line_lim = line_buf + 255;
	if (line_lim > endl_buf) line_lim = endl_buf;
	while (line_buf < line_lim && *line_buf == cdata)
	  line_buf++;
	prp = line_buf; line_buf = start_ptr;
	p_save = (prp - line_buf) - 3;
	if (line_buf != line_ptr && prp < line_lim0) p_save -= 2;
      }
      v_save = -3;
      {
	char *line_lim = line_buf + 255;
	while (line_buf < endl_buf &&
	       line_buf[0] == line_buf[buf_offs])
	{
	  if (line_buf >= line_lim)
	  {
	    v_save -= 3; line_lim += 255;
	  }
	  line_buf++;
      } }
      vrp = line_buf; line_buf = start_ptr;
      v_save += vrp - line_buf;
      if (line_buf != line_ptr && vrp < line_lim0) v_save -= 2;
    }
    while (s_save < 0 && p_save < 0 && v_save < 0 &&
	   ++line_buf < line_lim0);
    if (line_buf != line_ptr)
    {
      FPUTC(output, 0x80);
      FPUTC(output, (char)(line_buf - line_ptr));
      do FPUTC(output, *line_ptr++);
      while (line_ptr < line_buf);
    }
    if (s_save >= 0 && s_save >= p_save && s_save >= v_save)
    {
      char *line_lim = line_buf + 127;
      if (line_lim > endl_buf) line_lim = endl_buf;
      while (line_buf < line_lim && *line_buf == cdata) line_buf++;
      cdata <<= 7;
      FPUTC(output, cdata | (char)(line_buf - line_ptr));
      line_ptr = line_buf;
    }
    else if (p_save >= 0 && p_save >= v_save)
    {
      FPUTC(output, 0);
      FPUTC(output, (char)(prp - line_buf));
      FPUTC(output, cdata);
      line_ptr = line_buf = prp;
    }
    else if (v_save >= 0)
    {
      line_buf += 255;
      if (line_buf > vrp) line_buf = vrp;
      FPUTC(output, 0);
      FPUTC(output, 0);
      FPUTC(output, (char)(line_buf - line_ptr) - 1);
      line_ptr = line_buf;
  } }
  while (line_buf < endl_buf);

  ip->image_adr = ip->image_ptr;
}

static void level_3_putline(IBUF *ip)
{
  char *line_buf, *line_ptr, *endl_buf;
  FBUFPUB *output;

  line_buf = ip->line_adr;
  endl_buf = ip->pub.pbuf;
  output = ip->output;

  if (ip->pub.init == 0)
  {
    ip->pub.init++;
    line_ptr = endl_buf;
    /* Make a complement copy of the first line. This is a trick
     * to avoid special casing extended vrc checking for the first
     * line in the raw encoder.
     */
    do *line_ptr++ = ~*line_buf++;
    while (line_buf < endl_buf);
    goto mark3;
  }
  line_ptr = ip->image_ptr;
  do
    if (*line_ptr++ != *line_buf++)
    {
      if (ip->out_vrc != 1) goto mark1;
      goto mark2;
    }
  while (line_buf < endl_buf);

  do
  { if (ip->out_vrc == 256)
    {
      mark1:
      if (ip->image_ptr != ip->image_adr)
	(*ip->raw_encode)(ip);
      FPUTC(output, 0); FPUTC(output, 0);
      FPUTC(output, 0xFF);
      FPUTC(output, (char)ip->out_vrc);
      mark2:
      line_ptr = ip->image_ptr;
      line_ptr += ip->length;
      mark3:
      line_buf = ip->line_adr;
      ip->image_ptr = line_ptr;
      do *line_ptr++ = *line_buf++;
      while (line_buf < endl_buf);
      ip->out_vrc = 0;
    }
    ip->out_vrc++;
    if (--ip->height <= 0)
    {
      if (ip->out_vrc != 1)
      {
	if (ip->image_ptr != ip->image_adr)
	  (*ip->raw_encode)(ip);
	FPUTC(output, 0); FPUTC(output, 0);
	FPUTC(output, 0xFF);
	FPUTC(output, (char)ip->out_vrc);
      }
      ip->image_ptr += ip->length;
      (*ip->raw_encode)(ip);
      (*ip->user_exit)();
  } }
  while (--ip->pub.vrc);
  ip->pub.vrc++;

  ip->pub.pbuf = ip->line_adr; ip->pub.bytes_left = ip->length;
}

static void l_2_p_2_encode(IBUF *ip)
{
  char cdata, c_val, *line_buf, *line_ptr, *endl_buf;
  long s_save, p_save, v_save, buf_offs;
  char *srp, *prp, *vrp;
  FBUFPUB *output;

  output = ip->output;
  if (ip->out_vrc != 1)
  {
    FPUTC(output, 0); FPUTC(output, 0);
    FPUTC(output, 0xFF); FPUTC(output, (char)ip->out_vrc);
  }
  /* OK, here's the heart of the Level-2 encoder...
   * The outer loop (within full-line loop) is set up for
   * simple (uncompressed) bytestring handling.
   * "line_ptr" points to next byte to write, "line_buf"
   * is a preview pointer to check for possible _s_olid,
   * _p_attern, or _v_ertical run compression.
   */
  endl_buf = ip->image_adr; buf_offs = ip->length;
  line_ptr = line_buf = ip->pub.pbuf;
  do
  { /* "line_lim0" is set as limit for bytestring handler.
     * NOTE: The offset 256 is the maximum Level-2 bytestring
     * counter, which results in a zero byte value if reached.
     * For Level-1 (compatibility) the maximum offset is 255.
     */
    char *line_lim0 = line_buf + 256;
    if (line_lim0 > endl_buf) line_lim0 = endl_buf;
    do
    { char *start_ptr = line_buf;
      s_save = -1;
      if ((cdata = *line_buf) == 0 || cdata == -1)
      {
	char *line_lim = line_buf + 127;
	while (line_buf < endl_buf)
	{
	  if (*line_buf != cdata)
	  {
	    cdata = ~cdata;
	    if (*line_buf != cdata) break;
	    line_lim = line_buf;
	  }
	  if (line_buf >= line_lim)
	  {
	    s_save -= 1; line_lim += 127;
	  }
	  line_buf++;
	}
	cdata = *start_ptr;
      }
      srp = line_buf; line_buf = start_ptr;
      s_save += srp - line_buf;
      if (line_buf != line_ptr && srp < line_lim0) s_save -= 2;
      if (line_buf < endl_buf - 1)
      {
	char *line_lim = line_buf + 255 * 2;
	if (line_lim > endl_buf) line_lim = endl_buf;
	--line_lim;
	c_val = line_buf[1];
	while (line_buf < line_lim &&
	       line_buf[0] == cdata &&
	       line_buf[1] == c_val) line_buf += 2;
      }
      prp = line_buf; line_buf = start_ptr;
      p_save = (prp - line_buf) - 4;
      if (line_buf != line_ptr && prp < line_lim0) p_save -= 2;
      v_save = -3;
      {
	char *line_lim = line_buf + 255;
	while (line_buf < endl_buf &&
	       line_buf[0] == line_buf[buf_offs])
	{
	  if (line_buf >= line_lim)
	  {
	    v_save -= 3; line_lim += 255;
	  }
	  line_buf++;
      } }
      vrp = line_buf; line_buf = start_ptr;
      v_save += vrp - line_buf;
      if (line_buf != line_ptr && vrp < line_lim0) v_save -= 2;
    }
    while (s_save < 0 && p_save < 0 && v_save < 0 &&
	   ++line_buf < line_lim0);
    if (line_buf != line_ptr)
    {
      FPUTC(output, 0x80);
      FPUTC(output, (char)(line_buf - line_ptr));
      do FPUTC(output, *line_ptr++);
      while (line_ptr < line_buf);
    }
    if (s_save >= 0 && s_save >= p_save && s_save >= v_save)
    {
      char *line_lim = line_buf + 127;
      if (line_lim > endl_buf) line_lim = endl_buf;
      while (line_buf < line_lim && *line_buf == cdata) line_buf++;
      cdata <<= 7;
      FPUTC(output, cdata | (char)(line_buf - line_ptr));
      line_ptr = line_buf;
    }
    else if (p_save >= 0 && p_save >= v_save)
    {
      FPUTC(output, 0);
      FPUTC(output, (char)((prp - line_buf) >> 1));
      FPUTC(output, cdata);
      FPUTC(output, c_val);
      line_ptr = line_buf = prp;
    }
    else if (v_save >= 0)
    {
      line_buf += 255;
      if (line_buf > vrp) line_buf = vrp;
      FPUTC(output, 0);
      FPUTC(output, 0);
      FPUTC(output, (char)(line_buf - line_ptr) - 1);
      line_ptr = line_buf;
  } }
  while (line_buf < endl_buf);
  /*
   * End of Level-2 encoder.
   */

  line_buf = ip->pub.pbuf;
  line_ptr = endl_buf;
  do *line_ptr++ = *line_buf++;
  while (line_buf < endl_buf);
}

static void l_2_p_1_encode(IBUF *ip)
{
  char cdata, *line_buf, *line_ptr, *endl_buf;
  long s_save, p_save, v_save, buf_offs;
  char *srp, *prp, *vrp;
  FBUFPUB *output;

  output = ip->output;
  if (ip->out_vrc != 1)
  {
    FPUTC(output, 0); FPUTC(output, 0);
    FPUTC(output, 0xFF); FPUTC(output, (char)ip->out_vrc);
  }
  /* OK, here's the heart of the Level-2 encoder...
   * The outer loop (within full-line loop) is set up for
   * simple (uncompressed) bytestring handling.
   * "line_ptr" points to next byte to write, "line_buf"
   * is a preview pointer to check for possible _s_olid,
   * _p_attern, or _v_ertical run compression.
   */
  endl_buf = ip->image_adr; buf_offs = ip->length;
  line_ptr = line_buf = ip->pub.pbuf;
  do
  { /* "line_lim0" is set as limit for bytestring handler.
     * NOTE: The offset 256 is the maximum Level-2 bytestring
     * counter, which results in a zero byte value if reached.
     * For Level-1 (compatibility) the maximum offset is 255.
     */
    char *line_lim0 = line_buf + 256;
    if (line_lim0 > endl_buf) line_lim0 = endl_buf;
    do
    { char *start_ptr = line_buf;
      s_save = -1;
      if ((cdata = *line_buf) == 0 || cdata == -1)
      {
	char *line_lim = line_buf + 127;
	while (line_buf < endl_buf)
	{
	  if (*line_buf != cdata)
	  {
	    cdata = ~cdata;
	    if (*line_buf != cdata) break;
	    line_lim = line_buf;
	  }
	  if (line_buf >= line_lim)
	  {
	    s_save -= 1; line_lim += 127;
	  }
	  line_buf++;
	}
	cdata = *start_ptr;
      }
      srp = line_buf; line_buf = start_ptr;
      s_save += srp - line_buf;
      if (line_buf != line_ptr && srp < line_lim0) s_save -= 2;
      {
	char *line_lim = line_buf + 255;
	if (line_lim > endl_buf) line_lim = endl_buf;
	while (line_buf < line_lim && *line_buf == cdata)
	  line_buf++;
	prp = line_buf; line_buf = start_ptr;
	p_save = (prp - line_buf) - 3;
	if (line_buf != line_ptr && prp < line_lim0) p_save -= 2;
      }
      v_save = -3;
      {
	char *line_lim = line_buf + 255;
	while (line_buf < endl_buf &&
	       line_buf[0] == line_buf[buf_offs])
	{
	  if (line_buf >= line_lim)
	  {
	    v_save -= 3; line_lim += 255;
	  }
	  line_buf++;
      } }
      vrp = line_buf; line_buf = start_ptr;
      v_save += vrp - line_buf;
      if (line_buf != line_ptr && vrp < line_lim0) v_save -= 2;
    }
    while (s_save < 0 && p_save < 0 && v_save < 0 &&
	   ++line_buf < line_lim0);
    if (line_buf != line_ptr)
    {
      FPUTC(output, 0x80);
      FPUTC(output, (char)(line_buf - line_ptr));
      do FPUTC(output, *line_ptr++);
      while (line_ptr < line_buf);
    }
    if (s_save >= 0 && s_save >= p_save && s_save >= v_save)
    {
      char *line_lim = line_buf + 127;
      if (line_lim > endl_buf) line_lim = endl_buf;
      while (line_buf < line_lim && *line_buf == cdata) line_buf++;
      cdata <<= 7;
      FPUTC(output, cdata | (char)(line_buf - line_ptr));
      line_ptr = line_buf;
    }
    else if (p_save >= 0 && p_save >= v_save)
    {
      FPUTC(output, 0);
      FPUTC(output, (char)(prp - line_buf));
      FPUTC(output, cdata);
      line_ptr = line_buf = prp;
    }
    else if (v_save >= 0)
    {
      line_buf += 255;
      if (line_buf > vrp) line_buf = vrp;
      FPUTC(output, 0);
      FPUTC(output, 0);
      FPUTC(output, (char)(line_buf - line_ptr) - 1);
      line_ptr = line_buf;
  } }
  while (line_buf < endl_buf);
  /*
   * End of Level-2 encoder.
   */

  line_buf = ip->pub.pbuf;
  line_ptr = endl_buf;
  do *line_ptr++ = *line_buf++;
  while (line_buf < endl_buf);
}

static void level_2_putline(IBUF *ip)
{
  char *line_buf, *line_ptr, *endl_buf;

  line_buf = ip->line_adr;
  endl_buf = ip->pub.pbuf;

  if (ip->pub.init == 0)
  {
    ip->pub.init++;
    ip->out_vrc = 0;
    line_ptr = ip->image_adr;
    /* Make a complement copy of the first line. This is a trick
     * to avoid special casing extended vrc checking for the first
     * line in the raw encoder.
     */
    do *line_ptr++ = ~*line_buf++;
    while (line_buf < endl_buf);
    line_buf = ip->line_adr;
    line_ptr = endl_buf;
  }
  else
  {
    line_ptr = endl_buf;
    do
    { if (*line_ptr != *line_buf)
      {
	(*ip->raw_encode)(ip);
	ip->out_vrc = 0;
	break;
      }
      line_ptr++; line_buf++;
    }
    while (line_buf < endl_buf);
  }
  while (line_buf < endl_buf) *line_ptr++ = *line_buf++;

  do
  { if (ip->out_vrc == 256)
    {
      (*ip->raw_encode)(ip);
      ip->out_vrc = 0;
    }
    ip->out_vrc++;
    if (--ip->height <= 0)
    {
      (*ip->raw_encode)(ip);
      (*ip->user_exit)();
  } }
  while (--ip->pub.vrc);
  ip->pub.vrc++;

  ip->pub.pbuf = ip->line_adr; ip->pub.bytes_left = ip->length;
}

static void l_1_p_2_encode(IBUF *ip)
{
  char cdata, c_val, *line_buf, *line_ptr, *endl_buf, *start_ptr;
  FBUFPUB *output;
  int i;

  output = ip->output;
  if (ip->out_vrc != 1)
  {
    FPUTC(output, 0); FPUTC(output, 0);
    FPUTC(output, 0xFF); FPUTC(output, (char)ip->out_vrc);
  }
  line_ptr = line_buf = ip->pub.pbuf;
  do
  { endl_buf = line_buf + ip->byte_width;
    do
    { char *line_lim0 = line_buf + 255;
      if (line_lim0 > endl_buf) line_lim0 = endl_buf;
      do
      { if ((cdata = *line_buf++) == 0 || cdata == -1)
	{
	  char *line_lim = --line_buf + 127;
	  if (line_lim > endl_buf) line_lim = endl_buf;
	  start_ptr = line_buf;
	  while (line_buf < line_lim && *line_buf == cdata)
	    line_buf++;
	  i = (int)(line_buf - start_ptr);
	  if (i >= 3 || start_ptr == line_ptr ||
	      line_buf >= line_lim0)
	  {
	    entry:
	    if (start_ptr != line_ptr)
	    {
	      FPUTC(output, 0x80);
	      FPUTC(output, (char)(start_ptr - line_ptr));
	      do FPUTC(output, *line_ptr++);
	      while (line_ptr < start_ptr);
	    }
	    cdata <<= 7; cdata |= (char)i;
	    FPUTC(output, cdata);
	    line_ptr = line_buf = start_ptr + i;
	    break;
	  }
	  if (i == 2)
	    for (;;)
	    {
	      char cval2;
	      if ((c_val = *line_buf++) == 0 || c_val == -1)
	      {
		if (line_buf >= line_lim0) goto entry;
		if (*line_buf == c_val) goto entry;
	      }
	      else
	      {
		if (line_buf >= line_lim0 - 2) break;
		if (c_val != line_buf[1]) break;
		cval2 = line_buf[0];
		if (cval2 != line_buf[2]) break;
		if (line_buf >= line_lim0 - 3) goto entry;
		if (line_buf < line_lim0 - 4 &&
		    line_buf[3] == c_val &&
		    line_buf[4] == cval2) goto entry;
	    } }
	  line_buf = start_ptr + 1;
	}
	if (line_buf < endl_buf - 2 &&
	    cdata == line_buf[1])
	{
	  c_val = line_buf[0];
	  if (c_val == line_buf[2])
	  {
	    char *line_lim = --line_buf + 255 * 2;
	    if (line_lim > endl_buf) line_lim = endl_buf;
	    start_ptr = line_buf;
	    --line_lim;
	    while (line_buf < line_lim &&
		   line_buf[0] == cdata &&
		   line_buf[1] == c_val) line_buf += 2;
	    i = (int)(line_buf - start_ptr) >> 1;
	    if (i >= 3 || start_ptr == line_ptr ||
		line_buf >= line_lim0)
	    {
	      if (start_ptr != line_ptr)
	      {
		FPUTC(output, 0x80);
		FPUTC(output, (char)(start_ptr - line_ptr));
		do FPUTC(output, *line_ptr++);
		while (line_ptr < start_ptr);
	      }
	      FPUTC(output, 0);
	      FPUTC(output, (char)i);
	      FPUTC(output, cdata);
	      FPUTC(output, c_val);
	      line_ptr = line_buf; break;
	    }
	    line_buf = start_ptr + 1;
      } } }
      while (line_buf < line_lim0);
      if (line_buf != line_ptr)
      {
	FPUTC(output, 0x80);
	FPUTC(output, (char)(line_buf - line_ptr));
	do FPUTC(output, *line_ptr++);
	while (line_ptr < line_buf);
    } }
    while (line_buf < endl_buf);
  }
  while (endl_buf < ip->image_adr);
}

static void l_1_p_1_encode(IBUF *ip)
{
  char cdata, c_val, *line_buf, *line_ptr, *endl_buf, *start_ptr;
  FBUFPUB *output;
  int i;

  output = ip->output;
  if (ip->out_vrc != 1)
  {
    FPUTC(output, 0); FPUTC(output, 0);
    FPUTC(output, 0xFF); FPUTC(output, (char)ip->out_vrc);
  }
  line_ptr = line_buf = ip->pub.pbuf;
  do
  { endl_buf = line_buf + ip->byte_width;
    do
    { char *line_lim0 = line_buf + 255;
      if (line_lim0 > endl_buf) line_lim0 = endl_buf;
      do
      { if ((cdata = *line_buf++) == 0 || cdata == -1)
	{
	  char *line_lim = --line_buf + 127;
	  if (line_lim > endl_buf) line_lim = endl_buf;
	  start_ptr = line_buf;
	  while (line_buf < line_lim && *line_buf == cdata)
	    line_buf++;
	  i = (int)(line_buf - start_ptr);
	  if (i >= 3 || start_ptr == line_ptr ||
	      line_buf >= line_lim0)
	  {
	    put_solid:
	    if (start_ptr != line_ptr)
	    {
	      FPUTC(output, 0x80);
	      FPUTC(output, (char)(start_ptr - line_ptr));
	      do FPUTC(output, *line_ptr++);
	      while (line_ptr < start_ptr);
	    }
	    cdata <<= 7; cdata |= (char)i;
	    FPUTC(output, cdata);
	    line_ptr = line_buf = start_ptr + i;
	    break;
	  }
	  if (i == 2)
	    for (;;)
	      if ((c_val = *line_buf++) == 0 || c_val == -1)
	      {
		if (line_buf >= line_lim0) goto put_solid;
		if (*line_buf == c_val) goto put_solid;
	      }
	      else
	      {
		if (line_buf >= line_lim0 - 1) break;
		if (c_val != line_buf[0]) break;
		if (c_val != line_buf[1]) break;
		if (line_buf >= line_lim0 - 2) goto put_solid;
		if (c_val == line_buf[2]) goto put_solid;
	      }
	    line_buf = start_ptr + 1;
	}
	if (line_buf < endl_buf - 1 &&
	    cdata == line_buf[0] &&
	    cdata == line_buf[1])
	{
	  char *line_lim = --line_buf + 255;
	  if (line_lim > endl_buf) line_lim = endl_buf;
	  start_ptr = line_buf;
	  while (line_buf < line_lim && *line_buf == cdata)
	    line_buf++;
	  i = (int)(line_buf - start_ptr);
	  if (i >= 5 || start_ptr == line_ptr ||
	      line_buf >= line_lim0)
	  {
	    put_pattern:
	    if (start_ptr != line_ptr)
	    {
	      FPUTC(output, 0x80);
	      FPUTC(output, (char)(start_ptr - line_ptr));
	      do FPUTC(output, *line_ptr++);
	      while (line_ptr < start_ptr);
	    }
	    FPUTC(output, 0);
	    FPUTC(output, (char)i);
	    FPUTC(output, cdata);
	    line_ptr = line_buf = start_ptr + i;
	    break;
	  }
	  if (i == 4)
	    for (;;)
	      if ((c_val = *line_buf++) == 0 || c_val == -1)
	      {
		if (line_buf >= line_lim0) goto put_pattern;
		if (*line_buf == c_val) goto put_pattern;
	      }
	      else
	      {
		if (line_buf >= line_lim0 - 1) break;
		if (c_val != line_buf[0]) break;
		if (c_val != line_buf[1]) break;
		if (line_buf >= line_lim0 - 2) goto put_pattern;
		if (c_val == line_buf[2]) goto put_pattern;
	      }
	    line_buf = start_ptr + 1;
      } }
      while (line_buf < line_lim0);
      if (line_buf != line_ptr)
      {
	FPUTC(output, 0x80);
	FPUTC(output, (char)(line_buf - line_ptr));
	do FPUTC(output, *line_ptr++);
	while (line_ptr < line_buf);
    } }
    while (line_buf < endl_buf);
  }
  while (endl_buf < ip->image_adr);
}

static void level_1_putline(IBUF *ip)
{
  char *line_buf, *line_ptr, *endl_buf;

  line_buf = ip->line_adr;
  endl_buf = ip->pub.pbuf;
  line_ptr = endl_buf;

  if (ip->pub.init == 0)
  {
    ip->pub.init++;
    ip->out_vrc = 0;
  }
  else
    do
    { if (*line_ptr != *line_buf)
      {
	(*ip->raw_encode)(ip);
	ip->out_vrc = 0;
	break;
      }
      line_ptr++; line_buf++;
    }
    while (line_buf < endl_buf);

  while (line_buf < endl_buf) *line_ptr++ = *line_buf++;

  do
  { if (ip->out_vrc == 255)
    {
      (*ip->raw_encode)(ip);
      ip->out_vrc = 0;
    }
    ip->out_vrc++;
    if (--ip->height <= 0)
    {
      (*ip->raw_encode)(ip);
      (*ip->user_exit)();
  } }
  while (--ip->pub.vrc);
  ip->pub.vrc++;

  ip->pub.pbuf = ip->line_adr; ip->pub.bytes_left = ip->length;
}
