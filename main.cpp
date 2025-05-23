#include <SFML/Graphics.hpp>
#include <iostream>
#include "GraphHandle.hpp"

std::ostream& operator<<(std::ostream& os, const std::shared_ptr<NodoRed>& nodo) {
    if (nodo) {
        os << nodo->getname();  // usas el nombre personalizado
    } else {
        os << "nullptr";
    }
    return os;
}


std::ostream& operator<<(std::ostream& os, const ArcoInfo& arco) {
    os << "L:" << arco.distance;  // Muestra la longitud (ajusta según tus campos)
    // Si quieres más  detalles, puedes hacer:
    // os << "L:" << arco.length << ", R:" << arco.resistance;
    return os;
}


int main(){


    GraphHandle handler;
    GenericGraph<NodoRed> grafoRed; 
    GenericGraph<NodoRed> grafo_saved;
    float flash = 0;


    sf::RenderWindow window(sf::VideoMode(1000, 700), "Power Grid Planner");
    sf::Vector2f mapPosition(500, 350);

    sf::Font font;
    if (!font.loadFromFile("/usr/share/fonts/truetype/dejavu/DejaVuSans-Bold.ttf")) {
        std::cerr << "Error al cargar la fuente" << std::endl;
        return 1;
    }
    
    std::vector<Boton> botones = createButtons(font);
    std::vector<TrackNode> unconetedArcs;

    DynamicForm form(300, 300, 350, 150);


    std::string select = "";
    bool esperandoClick = false;
    sf::Vector2f clickPosicionMapa;


    sf::Texture texCasa, texSubestacion, texEstacion;
    if (!texCasa.loadFromFile("src/img/casa.png")) { 
        std::cerr << "Error al cargar textura casa.png" << std::endl; 
    }
    if (!texSubestacion.loadFromFile("src/img/subestacion.png")) { 
        std::cerr << "Error al cargar textura subestacion.png" << std::endl; 
    }
    if (!texEstacion.loadFromFile("src/img/estacion.png")) { 
        std::cerr << "Error al cargar textura estacion.png" << std::endl; 
    }
    std::map<std::string, sf::Texture> imgEstructure;

    imgEstructure["Casa"] = texCasa;
    imgEstructure["SubEstacion"] =texSubestacion;
    imgEstructure["Estacion"] = texEstacion;


    std::shared_ptr<NodoRed> seleccion1 = nullptr;
    std::shared_ptr<NodoRed> seleccion2 = nullptr;


    while (window.isOpen()){
        sf::Event event;

        while (window.pollEvent(event)){

            if (event.type == sf::Event::Closed){
                window.close();
            }
            


            if (form.visible()) {

                if (form.handleEvent(event, window)) {
                   
                    std::vector<std::string> datos = form.getFormData();
                  
                    agregarImagenSegunTipo( clickPosicionMapa, mapPosition , select, &texCasa, &texSubestacion, &texEstacion, datos, grafoRed, window);
                    esperandoClick = false;
                }else if(sf::Keyboard::isKeyPressed(sf::Keyboard::X)){
                    form.toggle();
                    esperandoClick = false;
                }
            } else if (event.type == sf::Event::MouseButtonPressed ) {

                sf::Vector2f mousePos = window.mapPixelToCoords(sf::Mouse::getPosition(window));

                std::string clickedButton = detectarClickBoton(botones, mousePos,mapPosition);
                
                if(clickedButton != ""){
                    unconetedArcs.clear();

                }

                if(event.mouseButton.button == sf::Mouse::Left){
                    if (!clickedButton.empty()) {
                        select = clickedButton;
                        esperandoClick = true;

                        // Manejo específico  para el nuevo botón
                        if (clickedButton == "C. Carga") {
                            
                            bool flaf_sub = false;
                            bool flaf_est = false;
                            grafoRed.for_each_node([&flaf_est, &flaf_sub](auto* node) {
                                if (node->get_info()->getTipo() == "Estacion") {
                                    flaf_est = true;
                                   
                                }

                                if(node->get_info()->getTipo() == "SubEstacion"){
                                    flaf_sub = true;
                                }
                            });

                            if(flaf_est && flaf_sub){
                                auto connectivity = handler.check_connectivity_estacion_subestacion(grafoRed);
                                if(std::get<0>(connectivity)){
                                    auto nodo =  handler.find_subestacion(grafoRed);
                                    auto grafo = handler.GraphHandlebfs_charge(grafoRed,nodo);
                                    unconetedArcs = handler.lookingNodes2(grafoRed, grafo, unconetedArcs, "0");
                                    //Designar::DotGraph<GenericGraph<NodoRed>>().write_graph(grafoRed, "grafoAlone3.dot");
                                }
                            }
                            
                           

                          

                        }else if(clickedButton == "C. Distancia"){
                            do{
                                unconetedArcs.clear();
                                unconetedArcs = handler.lookingNodes(grafoRed, unconetedArcs);
                            }while(unconetedArcs.size() > 0);
                            //Designar::DotGraph<GenericGraph<NodoRed>>().write_graph(grafoRed, "grafoAlone3.dot");
                        }else if(clickedButton == "Guardar"){
                            grafo_saved.clear();
                            grafo_saved = handler.Copiar_G(grafoRed);
                         
                            //Designar::DotGraph<GenericGraph<NodoRed>>().write_graph(grafoRed, "grafoAlone4.dot");

                        }else if(clickedButton == "Volver"){
                            grafoRed.clear();
                            grafoRed = handler.Copiar_G(grafo_saved);;
                            
                            //Designar::DotGraph<GenericGraph<NodoRed>>().write_graph(grafoRed, "grafoAlone5.dot");
                      

                        }else if(clickedButton == "Fix Distancia"){
                            handler.fix_charge(grafoRed);
                        }

                    }else if (esperandoClick && !select.empty()) {
                        //save the position for node
                        clickPosicionMapa = mousePos;
                        form.createForm(select);
                    }
                    
                }else{

                    if(select == "C. Manual"){
                        std::shared_ptr<NodoRed> seleccionado = detectImageClick( mousePos, mapPosition, grafoRed);
                        if (seleccionado != nullptr) {
                            if (seleccion1 == nullptr) {
                                seleccion1 = seleccionado;
                            } else if (seleccion2 == nullptr && seleccionado != seleccion1) {
                                seleccion2 = seleccionado;
                    
                                handler.connect_graph(grafoRed, seleccion1, seleccion2);
                                // Limpiar selección
                                seleccion1 = nullptr;
                                seleccion2 = nullptr;
                            }
                        }
                    }    
                }
                
            }

            
        }


         
        

    window.clear(sf::Color::Black);

    // Aplicar vista centrada
    window.setView(sf::View(mapPosition, sf::Vector2f(1000, 700)));

    moverView(mapPosition, botones);
    drawLandscape(window);
    Designar::DotGraph<GenericGraph<NodoRed>>().write_graph(grafoRed, "graphInt3.dot");

    drawArcsLines(window, grafoRed);
    //drawArcsLines(window, unconetedArcs, flash);

    drawImage(window, grafoRed);
    drawForm(window, form, mapPosition);
    drawButtons(window, botones);

    flash += 1;
    if(flash > 100) flash = 0;

    // Mostrar  info inferior
    sf::Vector2f mousePosMapa = window.mapPixelToCoords(sf::Mouse::getPosition(window));

    Designar::DigraphNode<std::shared_ptr<NodoRed>, ArcoInfo, Designar::EmptyClass> * img_selected = detectImageClick(mousePosMapa, grafoRed);


    if(img_selected != 0)showInfoBottom(window, font, img_selected, imgEstructure);

    window.display();
    }

    return 0;
}