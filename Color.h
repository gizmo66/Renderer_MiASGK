//
// Created by Adam on 2017-10-08.
//

#ifndef RENDERER_MIASGK_COLOR_H
#define RENDERER_MIASGK_COLOR_H


class Color {

public:
    Color(unsigned char r, unsigned char g, unsigned char b);

    explicit Color(Color *color);

    static const Color Black;
    static const Color Red;
    static const Color Green;
    static const Color LightBlue;
    unsigned char r;
    unsigned char g;
    unsigned char b;
private:
};


#endif //RENDERER_MIASGK_COLOR_H
