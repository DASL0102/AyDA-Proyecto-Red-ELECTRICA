#ifndef SUBESTACION_HPP
#define SUBESTACION_HPP
#include "NodoRed.hpp"

class SubEstacion : public NodoRed 
{
    private:
        std::string id;
        std::string name;
        double x, y;
        float maxCapacity;
        float currentCharge;
        std::string idEstacionPertenece;
        sf::Sprite sprite; 



    public:
        SubEstacion(std::string name,
            double x,
            double y,
            float maxCapacity,
            float currentCharge,
            std::string idEstacionPertenece,
            std::string id,
            sf::Texture* imgEstructure
    );
        
        
        std::string getTipo() const override;
        std::string getname() const override;
        double getX() const override;
        double getY() const override;
        sf::Sprite getSprite() const override;

        std::string getId() const;
        std::string getIdEstacionPertenece() const;

       
        float getmaxCapacity() const override;
        float getCurrentCharge() const override;
        void setIdEstacion(const std::string& newId);  
        void setCurrentCharge(const float& newcurrentCharge); 
        void setMaxCapacityCharge(const float& newmaxCharge); 
        std::shared_ptr<NodoRed> clone() const override; 
};

#endif 