#ifndef CASA_HPP
#define CASA_HPP
#include "NodoRed.hpp"

class Casa : public NodoRed
{
    private:
        std::string id;
        std::string name;
        double x, y;
        float consumption;
        std::string idSubEstacion;
        sf::Sprite sprite; 

    public:
        Casa(std::string name,
            double x,
            double y, 
            float consumption, 
            std::string idSubEstacion, 
            std::string id,
            sf::Texture*  imgEstructure
        );
       
        std::string getId() const;
        std::string getIdSubEstacion() const;
        std::string getTipo() const override;
        std::string getname() const override;
        double getX() const override;
        double getY() const override;
        sf::Sprite getSprite() const override;

        
        float getConsumption() const;  

               
        float getmaxCapacity() const override;
        float getCurrentCharge() const override;
        void setIdSubEstacion(const std::string& newId);  
        std::shared_ptr<NodoRed> clone() const override;
};

#endif 