/*
 * The layout of a font information block.
 * There is one of these for every loaded font or magnification thereof.
 * Duplicates are eliminated:  this is necessary because of possible recursion
 * in virtual fonts.
 *
 * Also note the strange units.  The design size is in 1/2^20 point
 * units (also called micro-points), and the individual character widths
 * are in the TFM file in 1/2^20 ems units, i.e., relative to the design size.
 *
 * We then change the sizes to SPELL units (unshrunk pixel / 2^16).
 */


#ifndef _FONT_H
#define _FONT_H

class dviWindow;
typedef	void	(dviWindow::*set_char_proc)(unsigned int, unsigned int);

#include <kprocess.h>
#include <qintdict.h>
#include <qstring.h>
#include <stdio.h>

#include "dviwin.h"
#include "glyph.h"


// Per character information for virtual fonts

class macro {
 public:
  macro(void);
  ~macro(void);

  unsigned char	*pos;		/* address of first byte of macro */
  unsigned char	*end;		/* address of last+1 byte */
  long	        dvi_adv;	/* DVI units to move reference point */
  bool          free_me;        // if memory at pos should be returned on destruction
};


class font : public QObject {
  Q_OBJECT

public:
  // Currently, kdvi supports fonts with at most 256 characters to
  // comply with "The DVI Driver Standard, Level 0". If you change
  // this value here, make sure to go through all the source and
  // ensure that character numbers are stored in ints rather than
  // unsigned chars.
  static const unsigned int max_num_of_chars_in_font = 256;
  enum font_flags {
    FONT_IN_USE  = 1,	// used for housekeeping
    FONT_LOADED  = 2,	// if font file has been read
    FONT_VIRTUAL = 4,	// if font is virtual 
    FONT_KPSE_NAME = 8 // if kpathsea has already tried to find the font name
  };
  

  font(const char *nfontname, double resolution_in_dpi, long chk, Q_INT32 scale, double pixelsPerDVIunit, class fontPool *pool, double shrinkFact, 
       double _enlargement, double _cmPerDVIunit);
  ~font();

  void reset(double resolution_in_dpi, double pixelPerDVIunit);


  // Members for character fonts
  glyph         *glyphptr(unsigned int ch);
  QPixmap        characterPixmap(unsigned int ch);
  void           setShrinkFactor(float sf);

  void           mark_as_used(void);
  double         cmPerDVIunit;
  class fontPool *font_pool;    // Pointer to the pool that contains this font.
  const char    *fontname;	// name of font, such as "cmr10"
  unsigned char  flags;		// flags byte (see values below)
  double         x_dimconv;	// size conversion factor
  double         enlargement;
  Q_INT32        scaled_size;   // Scaled size from the font definition command; in DVI units
  set_char_proc  set_char_p;	// proc used to set char

  // Resolution at which this font was rendered by MetaFont
  double         naturalResolution_in_dpi;

  FILE          *file;		// open font file or NULL
  QString        filename;	// name of font file

  glyph         *glyphtable;    // used by (loaded) raster fonts
  macro         *macrotable;    // used by (loaded) virtual fonts
  QIntDict<font> vf_table;      // used by (loaded) virtual fonts, list of fonts used by this vf, 
                                // acessible by number
  font          *first_font;	// used by (loaded) virtual fonts, list of fonts used by this vf

private:
  float         shrinkFactor;
  QPixmap       nullPixmap;
  QPixmap       *characterPixmaps[max_num_of_chars_in_font];
  Q_UINT32       checksum;

  // Functions related to virtual fonts
  void          read_VF_index(void );

  // Functions for pk fonts
  int           PK_get_nyb(FILE *fp);
  int           PK_packed_num(FILE *fp);
  void          PK_skip_specials(void);
  void          read_PK_char(unsigned int ch);
  void          read_PK_index(void);

public slots:
  void fontNameReceiver(QString);
};

#endif
