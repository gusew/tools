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
<?xml version=\"1.0\" encoding=\"UTF-8\" standalone=\"no\"?>\n\
<svg\n\
 xmlns:dc=\"http://purl.org/dc/elements/1.1/\"\n\
 xmlns:cc=\"http://creativecommons.org/ns#\"\n\
 xmlns:rdf=\"http://www.w3.org/1999/02/22-rdf-syntax-ns#\"\n\
 xmlns:svg=\"http://www.w3.org/2000/svg\"\n\
 xmlns=\"http://www.w3.org/2000/svg\"\n\
 version=\"1.1\"\n\
 id=\"svg2\"\n\
 viewBox=\"0 0 800 600\"\n\
 height=\"600px\"\n\
 width=\"800px\">\n\
<defs\n\
 id=\"defs4\" />\n\
<metadata\n\
 id=\"metadata7\">\n\
<rdf:RDF>\n\
<cc:Work\n\
 rdf:about=\"\">\n\
<dc:format>image/svg+xml</dc:format>\n\
<dc:type\n\
 rdf:resource=\"http://purl.org/dc/dcmitype/StillImage\" />\n\
<dc:title></dc:title>\n\
</cc:Work>\n\
</rdf:RDF>\n\
</metadata>\n\
<g\n\
 id=\"layer1\">\n\
";

const std::string SVG_FOOTER = "\
</g>\n\
</svg>\n\
";

#endif /* VISGRID_HPP */
