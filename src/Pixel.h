class Pixel {
public:

  Pixel(void);
  Pixel(const Pixel& pixel);
  Pixel(double r, double g, double b, double a);
  Pixel(const double rgba[4]);

  double Red(void) const;
  double Green(void) const;
  double Blue(void) const;
  double Alpha(void) const;

  double Luminance(void) const;

  void SetRed(double red);
  void SetGreen(double green);
  void SetBlue(double blue);
  void SetAlpha(double alpha);

  friend Pixel operator-(const Pixel& pixel1, const Pixel& pixel2);

private:

  double c[4];
};
