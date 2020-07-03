#include <SFML/Graphics.hpp>
#include <iostream>
using namespace sf;

/*public static Color ToColor(float r, float g, float b)
{
    return new Color((byte)(r * 255), (byte)(g * 255), (byte)(b * 255));
}

public static Color MakeDarken(Color color)
{
    var rgb = new[] { color.R / 255F, color.G / 255F, color.B / 255F };
    var hsv = RgbToHsv(rgb);
    hsv[2] *= 0.8F;
    rgb = HsvToRgb(hsv);
    return ToColor(rgb[0], rgb[1], rgb[2]);
}

public static float[] RgbToHsv(float[] value)
{
    var r = value[0];
    var g = value[1];
    var b = value[2];
    var min = r < g ? (r < b ? r : b) : (g < b ? g : b);
    var max = r > g ? (r > b ? r : b) : (g > b ? g : b);
    if (max == 0F)
    {
        // r = g = b = 0                // s = 0, v is undefined
        return new [] { -1F, 0F, max };
    }
    var delta = max - min;
    var v = max;
    var s = delta / max;
    var h = r == max ? (g - b) / delta :    // between yellow & magenta
        g == max ? 2F + (b - r) / delta :    // between cyan & yellow
        4F + (r - g) / delta;                // between magenta & cyan
    h *= 60F;                           // degrees
    h = h < 0 ? h + 360F : h;
    return new[] { h, s, v, };
}

public static float[] HsvToRgb(float[] value)
{
    var h = value[0];
    var s = value[1];
    var v = value[2];
    if (s == 0)
    {
        return new[] { v, v, v }; // achromatic (grey)
    }
    h /= 60F;                   // sector 0 to 5
    var i = (int)Math.Floor(h);
    var f = h - i;                      // factorial part of h
    var p = v * (1F - s);
    var q = v * (1F - s * f);
    var t = v * (1F - s * (1F - f));
    switch (i)
    {
        case 0: return new[] { v, t, p };
        case 1: return new[] { q, v, p };
        case 2: return new[] { p, v, t };
        case 3: return new[] { p, q, v };
        case 4: return new[] { t, p, v };
        default: return new[] { v, p, q };
    }
}*/

// hue: 0-360°; sat: 0.f-1.f; val: 0.f-1.f
sf::Color hsv(int hue, float sat, float val)
{
  hue %= 360;
  while(hue<0) hue += 360;

  if(sat<0.f) sat = 0.f;
  if(sat>1.f) sat = 1.f;

  if(val<0.f) val = 0.f;
  if(val>1.f) val = 1.f;

  int h = hue/60;
  float f = float(hue)/60-h;
  float p = val*(1.f-sat);
  float q = val*(1.f-sat*f);
  float t = val*(1.f-sat*(1-f));

  switch(h)
  {
    default:
    case 0:
    case 6: return sf::Color(val*255, t*255, p*255);
    case 1: return sf::Color(q*255, val*255, p*255);
    case 2: return sf::Color(p*255, val*255, t*255);
    case 3: return sf::Color(p*255, q*255, val*255);
    case 4: return sf::Color(t*255, p*255, val*255);
    case 5: return sf::Color(val*255, p*255, q*255);
  }
}
