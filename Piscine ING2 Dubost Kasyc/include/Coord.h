#ifndef COORD_H
#define COORD_H


class Coord
{
    public:
        Coord(double p_x, double p_y);
        virtual ~Coord();

        double getX() { return m_x; };
        double getY() { return m_y; };

    protected:

    private:
        double m_x;
        double m_y;
};

#endif // COORD_H
