#ifndef VISGRID_HPP
#define VISGRID_HPP

// define the image dimensions and should kept consistent with svg-header structure
const unsigned WIDTH = 800;
const unsigned HEIGHT = 600;
const unsigned MIN_DIM = (HEIGHT < WIDTH ? HEIGHT : WIDTH);

/** commands for formatting text as string:
 * s/\"/\\\"/g
 * s/\n/\\n\\\r/g
 */

const std::string SVG_HEADER = "\
<?xml version=\"1.0\" encoding=\"ISO-8859-1\" standalone=\"no\"?>\n\
<svg xmlns:svg=\"http://www.w3.org/2000/svg\" xmlns=\"http://www.w3.org/2000/svg\" xmlns:xlink=\"http://www.w3.org/1999/xlink\"\n\
 height=\"600px\" width=\"800px\">\n\
<title>Grid Visualization</title>\n\
<desc>This is a graphic visualization of a numerical grid</desc>\n\
";

const std::string SVG_DEFS_HEADER = "<defs>";
const std::string SVG_DEFS_FOOTER = "</defs>";

const std::string SVG_FOOTER = "\n</svg>";

#endif /* VISGRID_HPP */
