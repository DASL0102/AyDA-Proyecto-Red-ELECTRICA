#ifndef ESTACION_HPP
#define ESTACION_HPP
#include "NodoRed.hpp"
class Estacion : public NodoRed 
{
    private:
        std::string id;
        std::string name;
        double x, y;
        float maxCapacity;
        float currentCharge;
        sf::Sprite sprite; 

    public:
        Estacion(std::string name, 
            double x, 
            double y, 
            float maxCapacity, 
            float currentCharge, 
            std::string id, 
            sf::Texture* imgEstructure
        );
        
       
        std::string getId() const;
        std::string getTipo() const override;
        std::string getname() const override;
        double getX() const override;
        double getY() const override;
        sf::Sprite getSprite() const override;

       
        float getmaxCapacity() const override;
        float getCurrentCharge() const override;
        void setCurrentCharge(const float& newcurrentCharge);  
        std::shared_ptr<NodoRed> clone() const override; 
};

#endif 