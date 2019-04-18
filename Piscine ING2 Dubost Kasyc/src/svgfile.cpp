#include "svgfile.h"
#include <iostream>
#include <iomanip>
#include <sstream>

const std::string svgHeader =
    "<?xml version=\"1.0\" encoding=\"utf-8\"?>\n"
    "<svg xmlns=\"http://www.w3.org/2000/svg\"\n"
    "     xmlns:xlink=\"http://www.w3.org/1999/xlink\"\n"
    "     ";

const std::string svgEnding = "\n\n</svg>\n";


std::set<std::string> Svgfile::s_openfiles;

bool Svgfile::s_verbose = false;

Svgfile::Svgfile(std::string _filename, int _width, int _height) :
    m_filename{_filename}, m_width{_width}, m_height{_height}
{

    if (s_verbose)
        std::cout << "Opening SVG output file : "
                  << m_filename << std::endl;

    if ( s_openfiles.count(m_filename) )
        throw std::runtime_error( "File " + m_filename + " already open !" );

    m_ostrm.open(m_filename);
    s_openfiles.insert(m_filename);

    if (!m_ostrm)
    {
        std::cout << "Problem opening " << m_filename << std::endl;
        throw std::runtime_error("Could not open file " + m_filename );
    }

    if (s_verbose)
        std::cout << "OK" << std::endl;

    // Writing the header into the SVG file
    m_ostrm << svgHeader;
    m_ostrm << "width=\"" << m_width << "\" height=\"" << m_height << "\">\n\n";
}

Svgfile::~Svgfile()
{
    // Writing the ending into the SVG file
    m_ostrm << svgEnding;

    // Removing this file from the list of open files
    s_openfiles.erase(m_filename);

    // No need to explicitly close the ofstream object (automatic upon destruction)
}

// Helper templated function
template<typename T>
std::string attrib(std::string name, T val)
{
    std::ostringstream oss;
    oss << name << "=\"" << val << "\" ";
    return oss.str();
}

void Svgfile::addCircle(double x, double y, double r, double ep, std::string colorStroke, std::string colorFill)
{
    m_ostrm << "<circle "
            << attrib("cx", x)
            << attrib("cy", y)
            << attrib("r",  r)
            << attrib("fill", colorFill)
            << attrib("stroke", colorStroke )
            << attrib("stroke-width", ep )
            << "/>\n";
}

void Svgfile::addCircle(double x, double y, double r, std::string colorFill, double opacity)
{
    m_ostrm << "<circle "
            << attrib("cx", x)
            << attrib("cy", y)
            << attrib("r",  r)
            << attrib("fill", colorFill)
            << attrib("opacity", 1)
            << "/>\n";
}

void Svgfile::addLine(double x1, double y1, double x2, double y2, double r, std::string color)
{
    m_ostrm << "<line "
            << attrib("x1", x1)
            << attrib("y1", y1)
            << attrib("x2", x2)
            << attrib("y2", y2)
            << attrib("stroke", color)
            << attrib("stroke-width", r)
            << "/>\n";
}

void Svgfile::addCross(double x, double y, double span, std::string color)
{
    addLine(x-span, y-span, x+span, y+span, 1, color);
    addLine(x-span, y+span, x+span, y-span, 1,  color);
}

void Svgfile::addText(double x, double y, std::string text, std::string color, double font_size, bool center)
{
    /// <text x="180" y="60">Un texte</text>
    m_ostrm << "<text "
            << attrib("x", x)
            << attrib("y", y)
            << attrib("fill", color)
            << attrib("font-size", font_size);
    if (center)
    {
        m_ostrm << attrib("alignment-baseline", "middle")
                << attrib("text-anchor", "middle");
    }

    m_ostrm << ">" << text << "</text>\n";
}


void Svgfile::addGrid(double span, bool numbering, std::string color)
{
    //https://stackoverflow.com/questions/29200635/convert-float-to-string-with-precision-number-of-decimal-digits-specified

    double y=0;
    while (y<=m_height)
    {
        std::stringstream stream;
        stream << std::fixed << std::setprecision(2) << y;
        addLine(0, y, m_width, y, 1, color);
        if (numbering)
            addText(5, y-5, stream.str(), color);
        y+=span;
    }

    double x=0;
    while (x<=m_width)
    {
        std::stringstream stream;
        stream << std::fixed << std::setprecision(2) << x;
        addLine(x, 0, x, m_height, 1, color);
        if (numbering)
            addText(x+5, 15, stream.str(), color);
        x+=span;
    }
}


