#ifndef SVGFILE_H_INCLUDED
#define SVGFILE_H_INCLUDED

#include <string>
#include <fstream>
#include <set>

constexpr char defcol[] = "black";

class Svgfile
{
    public:
        Svgfile(std::string _filename = "output.svg", int _width=Svgfile::WIDTH, int _height=Svgfile::HEIGH);
        ~Svgfile();

        void addCircle(double x, double y, double r, double ep, std::string colorStroke = defcol, std::string colorFill = "none");
        void addCircle(double x, double y, double r, std::string colorFill=defcol, double opacity = 1.0);
        void addLine(double x1, double y1, double x2, double y2, double r, std::string color=defcol);
        void addCross(double x, double y, double span, std::string color=defcol);

        void addText(double x, double y, std::string text, std::string color=defcol, double font_size = 9.0, bool center = false);

        void addGrid(double span=100.0, bool numbering=true, std::string color="lightgrey");

        void addString(std::string _aAjouter);

        /// Type non copiable
        Svgfile(const Svgfile&) = delete;
        Svgfile& operator=(const Svgfile&) = delete;


        static const int WIDTH = 1920;
        static const int HEIGH = 1080;
        static bool s_verbose;
    private:
        std::string m_filename;
        std::ofstream m_ostrm;
        int m_width;
        int m_height;

        // Pour éviter les ouverture multiples
        static std::set<std::string> s_openfiles;
};

#endif // SVGFILE_H_INCLUDED



