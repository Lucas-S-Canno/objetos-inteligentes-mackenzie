# objetos-inteligentes-mackenzie
Repositório com os códigos do projeto da matéria de objetos inteligentes da Mackenzie

Este é um protótipo de um sistema básico de healthcare com transmissão de dados por meio da internet.
O sistema será uma integração de monitoramento básico coletando informações como temperatura, batimentos por minuto e oxigenação do sangue. A partir da coleta de dados essas informações serão transmitidas pela internet para uma outra pessoa, por exemplo um médico ou um sistema automatizado para a análise dos dados).
Neste projeto será usado como base o Arduino Uno, componentes eletrônicos como diodo, transistores e resistores, placa para montar os circuitos, fios para jumpers, display lcd e sensores com foco em uso médico.
Para este protótipo, tanto o arduino, quanto os componentes serão ligados através de uma protoboard onde as ligações serão feitas a partir de fios conectados com a função de interligar os componentes do projeto.

Componentes Utilizados:
Módulo Wifi - ESP8266
Resistores - 4.7k ohm e 3x10k ohm
Sensor de temperatura e umidade do ambiente - DHT11
Sensor de temperatura corporal - LM35 ou equivalente
Sensor de batimentos cardiacos - HW-827
Módulo Arduino UNO

Os códigos disponíveis neste repósitório são tanto do projeto final como para teste dos módulos separadamente.
  -Códigos para testes dos componentes:
    leitura_bpm -> teste do sensor HW-827
    leitura_temp_ambiente -> teste do sensor DHT11
    leitura_temp_corporal -> teste do sensor LM35
    conectando_esp8266 e esp_web_server -> testes do módulo esp8266

  -Código do projeto final:
    projeto_final -> projeto com protocolo MQTT
  

O projeto roda com o código projeto_final e com um Brocker MQTT como o Mosquitto, mas também é possivel interceptar esses valores resgatados pelo Brocker e usar em uma pagina web para a demonstração dos valores.
