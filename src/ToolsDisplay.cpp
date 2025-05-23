#include "ToolsDisplay.hpp"
#include <iostream>
#include <random>
#include <climits>


const int TILE_SIZE = 32;  
const int MAP_WIDTH = 50;  
const int MAP_HEIGHT = 40; 

const int FIX_POS_X = 500; 
const int FIX_POS_Y = 350; 

std::string generarIDUnico() {
    static std::random_device rd;
    static std::mt19937 gen(rd());
    static std::uniform_int_distribution<int> distrib(0, INT_MAX);
    
    return std::to_string(distrib(gen));
}


template <typename T>
//using GenericGraph = Designar::Digraph<std::shared_ptr<T>, double>;
using GenericGraph = Designar::Digraph<std::shared_ptr<T>, ArcoInfo>;

void drawLandscape(sf::RenderWindow& window) {
    for (int y = 0; y < MAP_HEIGHT; ++y) {
        for (int x = 0; x < MAP_WIDTH; ++x) {
            // Dibuja el tile verde
            sf::RectangleShape tile(sf::Vector2f(TILE_SIZE, TILE_SIZE));
            tile.setFillColor(sf::Color(0, 150, 0)); // Verde
            tile.setPosition(x * TILE_SIZE, y * TILE_SIZE);
            tile.setOutlineThickness(1.0f);
            tile.setOutlineColor(sf::Color::Black); 
            window.draw(tile);
        }
    }

}
    

    void drawButtons(sf::RenderWindow& window, const std::vector<Boton>& botones) {
        for (const auto& b : botones) {
            window.draw(b.rect);
            window.draw(b.texto);
        }
    }
    
    std::string detectarClickBoton(std::vector<Boton>& botones, sf::Vector2f mousePos, sf::Vector2f relativePos) {

        for (auto& boton : botones) {
       
            if (boton.rect.getGlobalBounds().contains(mousePos)) {
                // Free the buttons
                for (auto& b : botones) {
                    b.selected = false;
                    b.rect.setFillColor(sf::Color::White);
                }
                
                boton.selected = true;
                boton.rect.setFillColor(sf::Color(200, 220, 255)); 
                return boton.tipo;
            }
        }
        return "";
    }
    


    std::vector<Boton> createButtons(const sf::Font& font) {
        std::vector<Boton> botones;
        std::vector<std::string> nombres = {"Casa", "Subestacion", "Estacion", "Volver", "C. Distancia", "C. Carga", "C. Manual", "Guardar", "Fix Distancia"};
        
        for (int i = 0; i < nombres.size(); ++i) {
            Boton boton;
            boton.rect.setSize(sf::Vector2f(130, 30));
            
            // Primera fila: primeros 4 botones (i=0-3)
            if(i < 4) {
                boton.rect.setPosition(10 + i * 140, 10);
            } 
            // Segunda fila: últimos 4 botones (i=4-7)
            else {
                boton.rect.setPosition(10 + (i-4) * 140, 50); // 50 = 10 + 30 (altura) + 10 (margen)
            }
            
            boton.rect.setFillColor(sf::Color::White);
            boton.rect.setOutlineThickness(1);
            boton.rect.setOutlineColor(sf::Color(100, 100, 100));
            
            boton.tipo = nombres[i];
            boton.selected = false;
            
            boton.texto.setFont(font);
            boton.texto.setString(nombres[i]);
            boton.texto.setCharacterSize(16);
            boton.texto.setFillColor(sf::Color::Black);
            boton.texto.setPosition(boton.rect.getPosition().x + 10, boton.rect.getPosition().y + 5);
            
            botones.push_back(boton);
        }
        
        return botones;
    }
    
        void moverBotones(std::vector<Boton>& botones, int dx, int dy) {
            for (auto& b : botones) {

                b.rect.move(dx, dy);
                b.texto.move(dx, dy);
            }
        }
        
        

    void agregarImagenSegunTipo(
        sf::Vector2f mouseOne,
        sf::Vector2f relativePos,
        const std::string& tipo,
        sf::Texture * texCasa,
        sf::Texture * texSub,
        sf::Texture * texEst,
        const std::vector<std::string>& datos,
        GenericGraph<NodoRed>& grafoRed,
        sf::RenderWindow& window
    ) {
        sf::Vector2f mouseOne_ = window.mapPixelToCoords(sf::Mouse::getPosition(window));

        //sf::Vector2f mouseOne(posicion.x + relativePos.x - FIX_POS_X , posicion.y + relativePos.y - FIX_POS_Y);
        //std::cout<<"Mouse 1"<< mouseOne.x<<" "<<mouseOne.y <<std::endl;
        //std::cout<<"Mouse 1"<< mouseOne_.x<<" "<<mouseOne_.y <<std::endl;
        //std::cout<<"Mouse 1"<< posicion.x<<" "<<posicion.y <<std::endl;
        //std::cout<<"Mouse 1"<< relativePos.x<<" "<<relativePos.y <<std::endl;


        std::string Uid = generarIDUnico();
        std::cout<<"Llego hasta qui\n";
        if (tipo == "Casa") {

            std::string name = datos[0];
            std::string consumoStr = datos[1];
            //std::string idSubEstacion = datos[2];

            

            try {
                double consumo = std::stod(consumoStr);
                if(consumo > 1000000){
                    consumo = 1000000;
                }
                
                const auto& node8 = grafoRed.insert_node(std::make_shared<Casa>(
                    name, 
                    mouseOne.x, 
                    mouseOne.y, 
                    consumo,  
                    "0", 
                    Uid,
                    texCasa

                ));

             
                
            } catch (const std::exception& e) {
                std::cerr << "Error al convertir el consumo: " << e.what() << std::endl;
                double consumoDefault = 0.0;
                grafoRed.insert_node(std::make_shared<Casa>(
                    name, 
                    mouseOne.x, 
                    mouseOne.y, 
                    consumoDefault,
                    "0", 
                    Uid,
                    texCasa
                ));
            }    

        } else if (tipo == "Subestacion") {

            std::string name = datos[0];
            std::string capacidadMaxstr = datos[1];
            //std::string cargaActualstr = datos[2];
            //std::string idEstacionPertenece = datos[3];

            try {
                double capacidadMax = std::stod(capacidadMaxstr);
                //double cargaActual =  std::stod(cargaActualstr);
                if(capacidadMax > 1000000){
                    capacidadMax = 1000000;
                }
                
                grafoRed.insert_node(std::make_shared<SubEstacion>(
                    name, 
                    mouseOne.x, 
                    mouseOne.y, 
                    capacidadMax,  
                    capacidadMax,
                    "0", 
                    Uid,
                    texSub
                ));
                
            } catch (const std::exception& e) {
                std::cerr << "Error al convertir el consumo: " << e.what() << std::endl;

                double capacidadDefault = 0.0;
                //double cargaActualDefault = 0.0;

                grafoRed.insert_node(std::make_shared<SubEstacion>(
                    name, 
                    mouseOne.x, 
                    mouseOne.y, 
                    capacidadDefault,
                    capacidadDefault,
                    "0", 
                    Uid,
                    texSub
                ));
            }    

        } else if (tipo == "Estacion") {

            std::string name = datos[0];
            std::string capacidadMaxstr = datos[1];
            //std::string cargaActualstr = datos[2];
            
            try {
                double capacidadMax = std::stod(capacidadMaxstr);
                //double cargaActual =  std::stod(capacidadMaxstr);
                if(capacidadMax > 1000000){
                    capacidadMax = 1000000;
                }
                
                grafoRed.insert_node(std::make_shared<Estacion>(
                    name, 
                    mouseOne.x, 
                    mouseOne.y, 
                    capacidadMax, 
                    capacidadMax,
                    Uid,
                    texEst
                    
                ));
                
            } catch (const std::exception& e) {
                std::cerr << "Error al convertir el consumo: " << e.what() << std::endl;

                double capacidadDefault = 0.0;
                double cargaActualDefault = 0.0;
    
                grafoRed.insert_node(std::make_shared<Estacion>(
                    name, 
                    mouseOne.x, 
                    mouseOne.y, 
                    capacidadDefault,
                    cargaActualDefault,
                    Uid,
                    texEst
                ));
            }    


        } else {
            return;  
        }    
    }
 
    void moverView(sf::Vector2f& mapPosition, std::vector<Boton>& botones) {
        if (sf::Keyboard::isKeyPressed(sf::Keyboard::Up) and mapPosition.y > 1 + FIX_POS_Y) {
            mapPosition.y -= 2;
            moverBotones(botones, 0, -2);
        } else if (sf::Keyboard::isKeyPressed(sf::Keyboard::Down) and mapPosition.y < 930) {
            mapPosition.y += 2;
            moverBotones(botones, 0, +2);
        } else if (sf::Keyboard::isKeyPressed(sf::Keyboard::Left) and mapPosition.x > 1 + FIX_POS_X) {
            mapPosition.x -= 2;
            moverBotones(botones, -2, 0);
        } else if (sf::Keyboard::isKeyPressed(sf::Keyboard::Right) and mapPosition.x < 1100) {
            mapPosition.x += 2;
            moverBotones(botones, 2, 0);
        }
    }  
      

    void drawImage(sf::RenderWindow& window, GenericGraph<NodoRed>& grafoRed){
        grafoRed.for_each_node([&](auto* node) {
            if(node) window.draw(node->get_info()->getSprite());
        });
    }

    void drawForm(sf::RenderWindow& window,  DynamicForm form, sf::Vector2f mapPosition){
        if (form.visible()) {
                window.setView(window.getDefaultView());
                form.draw(window);
                window.setView(sf::View(mapPosition, sf::Vector2f(1000, 700))); 
        }
    }

    void showInfoBottom(sf::RenderWindow& window, const sf::Font& font, Designar::DigraphNode<std::shared_ptr<NodoRed>, ArcoInfo, Designar::EmptyClass>* seleccionada, std::map<std::string, sf::Texture> imgEstructure) {

        sf::RectangleShape panel(sf::Vector2f(window.getSize().x, 120));
        panel.setFillColor(sf::Color(30, 30, 30));
        panel.setPosition(0, window.getSize().y - 120);

        window.setView(window.getDefaultView());
        window.draw(panel);
        if (seleccionada->get_info() != nullptr) {
            //std::cout<<"hereeee\n";

            sf::Text titulo("Informacion de " + seleccionada->get_info()->getTipo(), font, 18);
            titulo.setPosition(20, window.getSize().y - 110);
            titulo.setFillColor(sf::Color::White);
            window.draw(titulo);
            
            std::vector<std::string> words;

            words.push_back(seleccionada->get_info()->getname());

            if(seleccionada->get_info()->getTipo() == "SubEstacion" ){

                if (auto sub_estacion = std::dynamic_pointer_cast<SubEstacion>(seleccionada->get_info())) {
                    
                    words.push_back("id SubEstacion: " + sub_estacion->getId());
                    words.push_back("id Estacion Pertenece: " + sub_estacion->getIdEstacionPertenece());
                } else {
                    std::cerr << "Dynamic_cast falló. ¿Es realmente una Casa?" << std::endl;
                }
                    
                
            }else if(seleccionada->get_info()->getTipo() == "Casa"){
                if (auto casa = std::dynamic_pointer_cast<Casa>(seleccionada->get_info())) {
                  
                    words.push_back("id SubEstacion Pertenece: " + casa->getIdSubEstacion());
                } else {
                    std::cerr << "Dynamic_cast falló. ¿Es realmente una Casa?" << std::endl;
                }
            }else if(seleccionada->get_info()->getTipo() == "Estacion"){
                if (auto estacion = std::dynamic_pointer_cast<Estacion>(seleccionada->get_info())) {
                  
                    words.push_back("id Estacion: " + estacion->getId());
                } else {
                    std::cerr << "Dynamic_cast falló. ¿Es realmente una Casa?" << std::endl;
                }
            }

            //std::cout << "Tipo real: " << typeid(seleccionada->get_info()).name() << std::endl;
            //std::cout << "Tipo real: " << typeid(seleccionada).name() << std::endl;
            //std::cout << "Tipo real: aaa" << seleccionada << std::endl;


            /*if(seleccionada->get_info()->getTipo() == "Estacion" or seleccionada->get_info()->getTipo() == "SubEstacion"){
                std::cout<<"hereeee\n";
                std::cout<<seleccionada->get_info()->getmaxCapacity()<<std::endl;

                words.push_back("Max Capacity : " + std::to_string(seleccionada->get_info()->getmaxCapacity()));
                std::cout<<"hereeee\n";

                words.push_back("Current Charge :"  + std::to_string(seleccionada->get_info()->getCurrentCharge()));
                std::cout<<"hereeee\n";

            }*/

            sf::Sprite sprite;
            sprite.setTexture(imgEstructure[seleccionada->get_info()->getTipo()]);
        
            
            sprite.setPosition(790, 520);  
            sprite.setScale(0.35f, 0.35f); 
            
            window.draw(sprite);


            for (size_t i = 0; i < words.size(); ++i) {
                sf::Text linea(words[i], font, 16);
                linea.setPosition(20, window.getSize().y - 80 + (i * 20));
                linea.setFillColor(sf::Color(200, 200, 200));
                window.draw(linea);
            }

        }
    }

    std::shared_ptr<NodoRed> detectImageClick(
        sf::Vector2f mousePos,
        sf::Vector2f relativePos,
        GenericGraph<NodoRed>&  grafoRed
    ){
        sf::Vector2f mouseOne(mousePos.x + relativePos.x - FIX_POS_X , mousePos.y + relativePos.y - FIX_POS_Y);
        std::shared_ptr<NodoRed> node_send = nullptr;
        grafoRed.for_each_node([&](auto* node) {
            if (node->get_info()->getSprite().getGlobalBounds().contains(mouseOne)) {
                node_send = node->get_info();
            }
        });

    return node_send;
    }


    Designar::DigraphNode<std::shared_ptr<NodoRed>, ArcoInfo, Designar::EmptyClass> * detectImageClick(
        sf::Vector2f mousePos,
        GenericGraph<NodoRed>&  grafoRed
    ){
        Designar::DigraphNode<std::shared_ptr<NodoRed>, ArcoInfo, Designar::EmptyClass>*  node_send = nullptr;
        grafoRed.for_each_node([&node_send, &mousePos](auto* node) {
            if (node->get_info()->getSprite().getGlobalBounds().contains(mousePos)) {
                node_send = node;
                //std::cout << "Tipo real: " << node << std::endl;
                //std::cout << "Tipo real: " << &node->get_info() << std::endl;
                //std::cout << "Tipo real: " << node_send->get_info() << std::endl;
                //std::cout << "Tipo real: " << node_send<< std::endl;
                //std::cout<<node_send->get_info()->getmaxCapacity()<<std::endl;
                //std::cout << "Tipo real: " << typeid(node).name() << std::endl;

            }
        });

    return node_send;
    }

    double distance(std::shared_ptr<NodoRed> a, std::shared_ptr<NodoRed> b) {
        float a_x = a->getX();
        float a_y = a->getY();
        float b_x = b->getX();
        float b_y = b->getY();
        return sqrt(pow(a_x - b_x, 2) + pow(b_y - a_y, 2));
    }


    sf::Vector2f moverPunto(sf::Vector2f inicio, sf::Vector2f fin, float t) {
        return inicio + t * (fin - inicio);
    }

    void drawArcsLines(sf::RenderWindow& window, GenericGraph<NodoRed>&  grafoRed){
        sf::CircleShape punto(2);
        punto.setFillColor(sf::Color::Red);
    
        grafoRed.for_each_arc([&window, &punto](auto* arc) {

            auto source = arc->get_src_node();
            auto target = arc->get_tgt_node();

            sf::Vector2f A(source->get_info()->getX() + 35, source->get_info()->getY() + 35);
            sf::Vector2f B(target->get_info()->getX() + 35, target->get_info()->getY()+ 35);

            //green line
            if(arc->get_info().costo){

                sf::Vertex linea[] = {
                    sf::Vertex(A, sf::Color::Green),
                    sf::Vertex(B, sf::Color::Green)
                };
                window.draw(linea, 4, sf::Lines);
                if (arc->get_info().perdidasEnergia < 1.0f) {

                    // mover el punto entre los dos nodos de la conexion para una vista de fluida de corriente
                    punto.setPosition(moverPunto(A, B, arc->get_info().perdidasEnergia));
                    arc->get_info().perdidasEnergia += 0.0009f;  
                    window.draw(punto);  
                }else{
                    arc->get_info().perdidasEnergia = 0;
                }

            //red line    
            }else{
                sf::Vertex linea[] = {
                    sf::Vertex(A, sf::Color::Red),
                    sf::Vertex(B, sf::Color::Red)
                };
                window.draw(linea, 4, sf::Lines);
            }
        
        });
    }

    void drawArcsLines(sf::RenderWindow& window, std::vector<TrackNode>& unconnetedArcs, float flash){
        sf::CircleShape punto(2);
        punto.setFillColor(sf::Color::Red);

        //std::cout<<"lite\n";
        for(auto  arc: unconnetedArcs) {
            //std::cout<<"inside\n";

            auto source = std::get<0>(arc);
                        //std::cout<<"inside\n";

            auto target = std::get<1>(arc);
                        //std::cout<<"inside\n";

            auto info = source->get_info();
           

        // std::cout<<source->get_info()->getTipo()<<"----node222##################\n";
        // std::cout<<source->get_info()->getname()<<"----node222##################\n";
        // std::cout<<source->get_info()->getX()<<"----node222##################\n";
        // std::cout<<source->get_info()->getY()<<"----node222##################\n";

            sf::Vector2f A(source->get_info()->getX() + 35, source->get_info()->getY() + 35);
            sf::Vector2f B(target->get_info()->getX() + 35, target->get_info()->getY()+ 35);


            //std::cout<<source->get_info()->getX() <<" ****** "<<source->get_info()->getY() <<std::endl;
            //std::cout<<target->get_info()->getX() <<" ****** "<<target->get_info()->getY() <<std::endl;
            //green line
            if(std::get<2>(arc).costo == 1){

                sf::Vertex linea[] = {
                    sf::Vertex(A, sf::Color::Green),
                    sf::Vertex(B, sf::Color::Green)
                };
                window.draw(linea, 4, sf::Lines);
                if (std::get<2>(arc).perdidasEnergia < 1.0f) {

                    // mover el punto entre los dos nodos de la conexion para una vista de fluida de corriente
                    punto.setPosition(moverPunto(A, B, std::get<2>(arc).perdidasEnergia));
                    std::get<2>(arc).perdidasEnergia += 0.0009f;  
                    window.draw(punto);  
                }else{
                    std::get<2>(arc).perdidasEnergia = 0;
                }

            //red line    
            }else if(std::get<2>(arc).costo == 0){
                sf::Vertex linea[] = {
                    sf::Vertex(A, sf::Color::Red),
                    sf::Vertex(B, sf::Color::Red)
                };
                window.draw(linea, 4, sf::Lines);
            }else if(std::get<2>(arc).costo == 2){
                sf::Vertex linea[] = {
                    sf::Vertex(A, sf::Color::Blue),
                    sf::Vertex(B, sf::Color::Blue)
                };
                
                if (flash < 10) {
                    // mover el punto entre los dos nodos de la conexion para una vista de fluida de corriente

                    window.draw(linea, 4, sf::Lines);

                }else if (flash> 20){
                    std::get<2>(arc).perdidasEnergia = 0;
                }

            }
        
        }
   
    
    }