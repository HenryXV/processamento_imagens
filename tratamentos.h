void valida_pixel(int &valor_pixel) {
    if(valor_pixel < 0)
        valor_pixel = 0;
    else if(valor_pixel > 255)
        valor_pixel = 255;
}