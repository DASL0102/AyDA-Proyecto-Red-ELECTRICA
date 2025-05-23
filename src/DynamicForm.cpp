#include "DynamicForm.hpp"

const int FIX_POS_X2 = 500; 
const int FIX_POS_Y2 = 350; 

DynamicForm::DynamicForm(float width, float height, float posX, float posY) {
    background.setSize(sf::Vector2f(width, height));
    background.setPosition(posX, posY);
    background.setFillColor(sf::Color(230, 230, 230, 220)); 
    background.setOutlineThickness(2);
    background.setOutlineColor(sf::Color(100, 100, 100));
    
    if (!font.loadFromFile("/usr/share/fonts/truetype/dejavu/DejaVuSans-Bold.ttf")) {
        std::cerr << "Error al cargar la fuente" << std::endl;
    }
    
    submitButton.setSize(sf::Vector2f(100, 30));
    submitButton.setFillColor(sf::Color(30, 180, 30));
    submitButtonText.setFont(font);
    submitButtonText.setString("Confirmar");
    submitButtonText.setCharacterSize(14);
    submitButtonText.setFillColor(sf::Color::White);
    
    isVisible = false;
}

void DynamicForm::clearFields() {
    labels.clear();
    inputFields.clear();
    inputTexts.clear();
    activeField = -1;
}

void DynamicForm::addField(const std::string& label, float yOffset) {
    sf::Text text;
    text.setFont(font);
    text.setString(label);
    text.setCharacterSize(16);
    text.setFillColor(sf::Color::Black);
    text.setPosition(background.getPosition().x + 15, 
                     background.getPosition().y + yOffset);
    labels.push_back(text);
    
    sf::RectangleShape field(sf::Vector2f(background.getSize().x - 30, 25));
    field.setPosition(background.getPosition().x + 15, 
                      background.getPosition().y + yOffset + 25);
    field.setFillColor(sf::Color::White);
    field.setOutlineThickness(1);
    field.setOutlineColor(sf::Color(150, 150, 150));
    inputFields.push_back(field);
    
    sf::Text inputText;
    inputText.setFont(font);
    inputText.setString("");
    inputText.setCharacterSize(16);
    inputText.setFillColor(sf::Color::Black);
    inputText.setPosition(field.getPosition().x + 5, field.getPosition().y + 2);
    inputTexts.push_back(inputText);
}

void DynamicForm::createForm(const std::string& formType) {
    clearFields();
    currentFormType = formType;
    
    if (formType == "Casa") {
        addField("name:", 20);
        addField("Consumo estimado (kW):", 120);
        // addField("id SubEstacion:", 70);
    }
    else if (formType == "Subestacion") {
        addField("name:", 20);
        addField("capacidadMax:", 70);
        //addField("cargaActual", 120);
        // addField("idEstacionPertenece", 170);

    }
    else if (formType == "Estacion") {
        addField("name:", 20);
        addField("capacidadMax:", 70);
        //addField("cargaActual:", 120);
    }
    
    //Ajustar posición del botón de envío
    float lastFieldY = background.getPosition().y + 120 + 25 + 20;
    submitButton.setPosition(background.getPosition().x + background.getSize().x/2 - 50, lastFieldY + 80);
    submitButtonText.setPosition(submitButton.getPosition().x + 10, submitButton.getPosition().y + 8 );

    
    
    show();
}

bool DynamicForm::handleEvent(const sf::Event& event, sf::RenderWindow& window) {
    if (!isVisible) return false;
    
    if (event.type == sf::Event::MouseButtonPressed) {
        if (event.mouseButton.button == sf::Mouse::Left) {
            //sf::Vector2f mousePos = window.mapPixelToCoords(sf::Mouse::getPosition(window));


            sf::Vector2i mouseOne1 = sf::Mouse::getPosition(window);
            sf::Vector2f mouseOne(mouseOne1.x, mouseOne1.y); 

            //sf::Vector2f mousePos = window.mapPixelToCoords(sf::Vector2i(event.mouseButton.x, event.mouseButton.y));
            //sf::Vector2f mouseOne(mousePos.x + relativePos.x - FIX_POS_X2 , mousePos.y + relativePos.y - FIX_POS_Y2);

            //std::cout << "Mouse Position: (" << mousePos.x << ", " << mousePos.y << ")" << std::endl;
            //std::cout << "Mouse Position: (" << mouseOne.x << ", " << mouseOne.y << ")" << std::endl;
            //std::cout << "Mouse Position: (" << mouseOne.x - mousePos.x << ", " << mouseOne.y - mousePos.y<< ")" << std::endl;

            activeField = -1;
            for (size_t i = 0; i < inputFields.size(); ++i) {

            sf::FloatRect bounds = inputFields[i].getGlobalBounds();
            //std::cout << " - Posición: (" << bounds.left << ", " << bounds.top << ")" << std::endl;
            //std::cout << " - Tamaño: " << bounds.width << "x" << bounds.height << std::endl;
            //std::cout << " - Bounds: [" << bounds.left << "," << bounds.top 
            //        << " to " << bounds.left+bounds.width << "," << bounds.top+bounds.height << "]" << std::endl;

            // Verificar clic en los campos de entrada
                if (inputFields[i].getGlobalBounds().contains(mouseOne)) {
                    activeField = static_cast<int>(i);
                    inputFields[i].setOutlineColor(sf::Color::Blue);
                    inputFields[i].setOutlineThickness(2);
                } else {
                    inputFields[i].setOutlineColor(sf::Color(150, 150, 150));
                    inputFields[i].setOutlineThickness(1);
                }
            }
            
            if (submitButton.getGlobalBounds().contains(mouseOne)) {
                bool allFieldsFilled = true;
                int j = 0;
                for (const auto& text : inputTexts) {
                    if (text.getString().isEmpty()) {
                        allFieldsFilled = false;
                        break;
                    }
                    bool flag = isNumeric(text.getString());
                    if(!flag){
                        allFieldsFilled = false;
                        inputFields[j].setOutlineColor(sf::Color::Red);
                        break;
                    }
                    j++;
                }
                
                if (allFieldsFilled) {
                    hide(); 
                    return true; 
                } else {
                    // Resaltar campos vacíos
                    for (size_t i = 0; i < inputTexts.size(); ++i) {
                        if (inputTexts[i].getString().isEmpty()) {
                            inputFields[i].setOutlineColor(sf::Color::Red);
                            inputFields[i].setOutlineThickness(2);
                        }
                    }
                }
            }
        }
    }
    
    if (event.type == sf::Event::TextEntered && activeField != -1) {
        if (event.text.unicode == '\b') { 
            if (!inputTexts[activeField].getString().isEmpty()) {
                std::string str = inputTexts[activeField].getString();
                str.pop_back();
                inputTexts[activeField].setString(str);
            }
        }
        else if (event.text.unicode < 128 && event.text.unicode != '\r' && event.text.unicode != '\n') {
            std::string str = inputTexts[activeField].getString();
            str += static_cast<char>(event.text.unicode);
            inputTexts[activeField].setString(str);
        }
    }
    
    return false;
}

void DynamicForm::show() { isVisible = true; }

void DynamicForm::hide() { isVisible = false; }

void DynamicForm::toggle() { isVisible = !isVisible; }

bool DynamicForm::visible() const { return isVisible; }

std::string DynamicForm::getFormType() const { return currentFormType; }

void DynamicForm::draw(sf::RenderWindow& window) {
    if (!isVisible) return;
    
    window.draw(background);
    
    for (auto& label : labels) {
        window.draw(label);
    }
    
    for (auto& field : inputFields) {
        window.draw(field);
    }
    
    for (auto& text : inputTexts) {
        window.draw(text);
    }
    
    window.draw(submitButton);
    window.draw(submitButtonText);
}


std::vector<std::string> DynamicForm::getFormData() const {
    std::vector<std::string> data;
    for (const auto& text : inputTexts) {
        data.push_back(text.getString());
    }
    return data;
}


bool DynamicForm::isNumeric(const std::string& str) {
    if (str.empty()) return false;
    

   
        if(str[0] == '-'){
            if (str[1] >= '0' && str[1] <= '9') {
                return false;
            }
           
        }
        
       return true;
}