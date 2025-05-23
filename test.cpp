#include <SFML/Graphics.hpp>
#include <iostream>
//#include "ToolsDisplay.hpp"
#include "GraphHandle.hpp"


sf::Vector2f moverPunto(sf::Vector2f inicio, sf::Vector2f fin, float t) {
    return inicio + t * (fin - inicio);
}

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
    // Si quieres más detalles, puedes hacer:
    // os << "L:" << arco.length << ", R:" << arco.resistance;
    return os;
}


int main(){

    GenericGraph<NodoRed> grafoRed; 

    sf::Texture textura_prueba;
    if (!textura_prueba.loadFromFile("src/img/casa.png")) {
        std::cerr << "Error al cargar textura casa.png" << std::endl;
    }

    // test

    const auto& node1 = grafoRed.insert_node(
        std::make_shared<Estacion>("E1", 5.0, 20.0, 5000.0, 0.0, "1111", &textura_prueba)
        //                                                              ^ Aquí pasamos la dirección
    );

    const auto& node2 = grafoRed.insert_node(
        std::make_shared<SubEstacion>("S1", 2.0, 15.0, 5000.0, 0.0, "1111", "2222", &textura_prueba)
        //                                                              ^ Aquí pasamos la dirección
    );

    const auto& node3 = grafoRed.insert_node(
        std::make_shared<SubEstacion>("S2", 15.0, 15.0, 5000.0, 0.0, "55", "44", &textura_prueba)
        //                                                              ^ Aquí pasamos la dirección
    );

    const auto& node4 = grafoRed.insert_node(
        std::make_shared<Casa>("C1", 1.0, 10.0, 5000.0, "2222", "3333", &textura_prueba)
        //                                                              ^ Aquí pasamos la dirección
    );

    const auto& node5 = grafoRed.insert_node(
        std::make_shared<Casa>("C2", 2.0, 8.0, 5000.0, "2222", "4444", &textura_prueba)
        //                                                              ^ Aquí pasamos la dirección
    );

    const auto& node6 = grafoRed.insert_node(
        std::make_shared<Casa>("C3", 0.5, 5.0, 5000.0, "2222", "4444", &textura_prueba)
        //                                                              ^ Aquí pasamos la dirección
    );

    const auto& node7 = grafoRed.insert_node(
        std::make_shared<Casa>("C4", 15.0, 10.0, 5000.0, "2222", "4444", &textura_prueba)
        //                                                              ^ Aquí pasamos la dirección
    );

    const auto& node8 = grafoRed.insert_node(
        std::make_shared<Casa>("C5", 20.0, 15.0, 5000.0, "2222", "4444", &textura_prueba)
        //                                                              ^ Aquí pasamos la dirección
    );

    const auto& node9 = grafoRed.insert_node(
        std::make_shared<Casa>("C6", 20.3, 16.1, 5000.0, "2222", "4444", &textura_prueba)
        //                                                              ^ Aquí pasamos la dirección
    );

   




    // Designar::DigraphNode<std::shared_ptr<NodoRed>, ArcoInfo, Designar::EmptyClass>* nodo_g2; 

    ArcoInfo arco = {25.6, 0.0, 0.0}; 

    grafoRed.insert_arc(node1, node2, arco);

    arco = {12, 0.0, 0.0}; 
    grafoRed.insert_arc(node1, node3, arco);

    // arco = {2, 0.0, 0.0}; 
    // grafoRed.insert_arc(node2, node4, arco);
    // arco = {4, 0.0, 0.0}; 
    // grafoRed.insert_arc(node4, node5, arco);
    // arco = {6, 0.0, 0.0}; 
    // grafoRed.insert_arc(node5, node6, arco);
    // arco = {1, 0.0, 0.0}; 
    // grafoRed.insert_arc(node3, node7, arco);
    // arco = {2, 0.0, 0.0}; 
    // grafoRed.insert_arc(node7, node8, arco);

    GraphHandle handler; // Crear una instancia de la clase
    // auto result = handler.min_connectivity(grafoRed, node9); // Llamar al método

    // auto node1_insert = std::get<0>(result);  // Primer elemento
    // auto node2_insert = std::get<1>(result);  // Segundo elemento
    // auto arc_insert   = std::get<2>(result);  // Tercer elemento

    // auto node_insert1 = grafoRed.search_node([&](auto node) {
    //     auto nodeInfo = node->get_info();
    //     return node1_insert->get_info() == nodeInfo;  // Comparar la información de los nodos
    // });

    // auto node_insert2 = grafoRed.search_node([&](auto node) {
    //     auto nodeInfo = node->get_info();
    //     return node2_insert->get_info() == nodeInfo;  // Comparar la información de los nodos
    // });

    // grafoRed.insert_arc(node_insert2, node_insert1, arc_insert);


    // grafoRed.for_each_node([&handler, &grafoRed](Designar::DigraphNode<std::shared_ptr<NodoRed>, ArcoInfo, Designar::EmptyClass>* node) {
    //     handler.best_connectivity(grafoRed, node);
    // });

    //handler.best_connectivity2(grafoRed);
   
    //handler.remove_connection(grafoRed);


   

    auto connected = handler.check_connectivity_estacion_subestacion(grafoRed);

    bool con = std::get<0>(connected);  // Primer elemento
    std::string message = std::get<1>(connected);  // Segundo elemento


    std::cout << "Graph is connected: " << (con ? "true" : "false") << std::endl;
    std::cout << "Message: " << message << std::endl;
    Designar::DotGraph<GenericGraph<NodoRed>>().write_graph(grafoRed, "graphInt3.dot");
    


    // fin test *************
    return 0;
}