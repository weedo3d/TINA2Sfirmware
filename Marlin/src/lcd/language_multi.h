/**
* Copyright (C) 2022 WEEDO3D Perron
*/

/**
* 多国语言字符定义
* 数组中元素对应语言如下：
* Multi-language character definition
* The corresponding language of the elements in the array is as follows:
* 0 - EN / English
* 1 - CN / Chinese
* 2 - FR / French
* 3 - DE / German
* 4 - ES / Spanish
* 5 - IT / Italy
* 6 - KANA / Japanese
* 7 - PT / Portugal
* 8 - NL / Dutch
* 9 - TR / Turkish
* 10- KR / Korea
* 11- RU / Russian
*/

#ifndef LANGUAGE_MULTI_H
#define LANGUAGE_MULTI_H

const char PERCENT_STRING[] PROGMEM = { "%d%%" };

const char MWELCOME_MSG[12][29] PROGMEM = {
	" Ready.",
	" 准备就绪",
	" Prete.",
	" bereit",
	" lista",
	" pronto.",
	" 準備完了",
	" pronta.",
	" gereed.",
	" hazir.",
	" 준비.",		
	" готов."
};

const char MMSG_YES[12][14] PROGMEM = {
	"Yes",
	"是",
	"Oui",
	"JA",
	"SI",
	"SI",
	"はい",
	"SI",
	"Ja",
	"EVET",
	"예",
	"Да",
};

const char MMSG_NO[12][14] PROGMEM = {
	"No",
	"否",
	"Non",
	"NEIN",
	"NO",
	"NO",
	"いいえ",
	"NO",
	"Nee",
	"HAYIR",
	"아니요",
	"Нет",
};

const char MMSG_BACK[12][13] PROGMEM = {
	"Back",
	"返回",
	"Retour",
	"Zurück",
	"Atrás",
	"Indietro",
	"戻り",
	"Atrás",
	"Rug",
	"Geri",
	"뒤로",
	"Назад",
};

const char MMSG_LCD_ENDSTOPS[12][22] PROGMEM = {
	"Endstops",
	"行程开关信息",		
	"Butées",
	"Endstopp",
	"Endstops",
	"Finecor.",
	"エンドストップ",
	"Fim de curso",
	"Endstops",
	"Enstops",
	"엔드스탑",
	"Эндстопы",
};

const char MMSG_MAIN[12][20] PROGMEM = {
	"Main",
	"主菜单", 		
	"Menu principal",
	"Hauptmenü",
	"Menú principal",
	"Menu principale",
	"メイン",
	"Menu principal",
	"Hoofdmenu",
	"Ana",
	"뒤로",
	"Меню",
};

const char MMSG_DISABLE_STEPPERS[12][32] PROGMEM = {
	"Disable steppers",
	"关闭步进电机",
	"Arrêter moteurs",
	"Motoren deaktivieren",
	"Apagar motores",
	"Disabilita Motori",
	"モーター電源オフ",
	"Desactivar motores",
	"Motoren uit",
	"Motorlari Durdur",
	"모터 정지",
	"Выкл. двигатели",
};

const char MMSG_AUTO_HOME[12][27] PROGMEM = {
	"Auto home",
	"回原点",
	"Origine auto",
	"Home",
	"Llevar al origen",
	"Auto Home",
	"原点復帰",
	"Ir para origem",
	"Auto home",
	"Eksenleri Sifirla",
	"오토홈",
	"Авто парковка",
};

const char MMSG_MOVE_AXIS[12][32] PROGMEM = {
	"Move axis",
	"点动模式",		
	"Déplacer un axe",
	"Achsen bewegen",
	"Mover ejes",
	"Muovi Asse",
	"軸移動",
	"Mover eixo",
	"As verplaatsen",
	"Eksen Yonet",
	"축 이동",		
	"Движение по осям",
};

const char MMSG_LEVEL_BED[12][33] PROGMEM = {
	"Level bed",
	"自动调平",
	"Niveau du lit",
	"Bett nivellieren",
	"Nivelar Cama",
	"Livella piano",
	"ベッドレベリング",
	"Cama nivelada",
	"Level bed",
	"Seviye yatagi",
	"수평 침대",		
	"Калибровать стол",
};

const char MMSG_PROBING_MESH[12][27] PROGMEM = {
	"Probing Point",
	"探测点",
	"Mesure point",
	"Messpunkt",
	"Sondear Punto",
	"Punto sondato",
	"プローブポイント",
	"Sondear Punto",
	"Indringend punt",
	"Prop Noktası",
	"프로빙 포인트",
	"",
};

const char MMSG_LED_CONTROL[12][23] PROGMEM = {
	"LED Control",
	"LED开关设置",		
	"Contrôle LED",
	"Licht-Steuerung",
	"Control LED",
	"Controllo LED",
	"LED コントロール",
	"Controle do LED",
	"LED Control",
	"LED Kontrolu",
	"LED 제어",
	"СВЕТОДИОД",
};

const char MMSG_MOVING[12][22] PROGMEM = {
	"Moving...",
	"正在移动...",		
	"Déplacement...",
	"In Bewegung...",
	"Moviendo...",
	"In movimento...",
	"移動中...",			
	"Mover-se ...",
	"Verplaatsen...",
	"Konumlaniyor...",
	"움직이는",
	"Движемся...",
};


const char MMSG_MOVE_X[12][25] PROGMEM = {
	"Move X",
	"移动X轴",		
	"Déplacer X",
	"Bewege X",
	"Mover X",
	"Muovi X",
	"X軸移動",
	"Mover X",
	"Verplaats X",
	"X",
	"X 이동",
	"Движение по X",
};

const char MMSG_MOVE_Y[12][25] PROGMEM = {
	"Move Y",
	"移动Y轴",
	"Déplacer Y",
	"Bewege Y",
	"Mover Y",
	"Muovi Y",
	"Y軸移動",
	"Mover Y",
	"Verplaats Y",
	"Y",
	"Y 이동",
	"Движение по Y",
};

const char MMSG_MOVE_Z[12][25] PROGMEM = {
	"Move Z",
	"移动Z轴",
	"Déplacer Z",
	"Bewege Z",
	"Mover Z",
	"Muovi Z",
	"Z軸移動",
	"Mover Z",
	"Verplaats Z",
	"Z",
	"Z 이동",
	"Движение по Z",
};

const char MMSG_MOVE_E[12][25] PROGMEM = {
	"Extruder",
	"挤出器",
	"Extrudeur",
	"Extruder",
	"Extrusor",
	"Estrusore",
	"エクストルーダー",
	"Mover Extrusor",
	"Extruder",
	"Ekstruder",
	"압출기",
	"Экструдер",
};

const char MMSG_MOVE_01MM[12][26] PROGMEM = {
	"Move 0.1mm",
	"移动0.1mm",
	"Déplacer 0.1mm",
	"0.1mm",
	"Mover 0.1mm",
	"Muovi di 0.1mm",
	"0.1mm移動",
	"Mover 0.1mm",
	"Verplaats 0.1mm",
	"0.1mm",
	"0.1mm 이동",		
	"Движение 0.1мм",
};

const char MMSG_MOVE_1MM[12][24] PROGMEM = {
	"Move 1mm",
	"移动1mm",
	"Déplacer 1mm",
	"1mm",
	"Mover 1mm",
	"Muovi di   1mm",
	"1mm移動",
	"Mover 1mm",
	"Verplaats 1mm",
	"1mm",
	"1mm 이동",
	"Движение 1мм",
};

const char MMSG_MOVE_10MM[12][25] PROGMEM = {
	"Move 10mm",
	"移动10mm",
	"Déplacer 10 mm",
	"10mm",
	"Mover 10mm",
	"Muovi di  10mm",
	"10mm移動",
	"Mover 10mm",
	"Verplaats 10mm",
	"10mm",
	"10mm 이동",
	"Движение 10мм",
};

const char MMSG_SPEED[12][18] PROGMEM = {
	"Speed",
	"速度",
	"Vitesse",
	"Geschw.",
	"Velocidad",
	"Velocità",
	"速度",
	"Velocidade",
	"Snelheid",
	"Hiz",
	"속도",
	"Скорость",
};


const char MMSG_NOZZLE[12][12] PROGMEM = {
	"Nozzle",
	"喷头",
	"Buse",
	"Düse",
	"Boquilla",
	"Ugello",
	"ノズル",
	"Bico",
	"Nozzle",
	"Nozul",
	"노즐",
	"Сопло",
};

const char MMSG_BED[12][12] PROGMEM = {
	"Bed",
	"热床",
	"Lit",
	"Bett",
	"Cama",
	"Piatto",
	"ベッド",
	"Base",
	"Bed",
	"Tabla",
	"베드",
	"Стол",
};

const char MMSG_FAN_SPEED[12][19] PROGMEM = {
	"Fan speed",
	"风扇速度",
	"Vit.  ventil.",
	"Lüfter",
	"Ventilador",
	"Vel. ventola",
	"ファン速度",
	"Vel. ventoinha",
	"Fan snelheid",
	"Fan Hizi",
	"펜 속도",
	"Кулер",
};

const char MMSG_FLOW[12][22] PROGMEM = {
	"Flow",
	"挤出速度",
	"Flux",
	"Flussrate",
	"Flujo",
	"Flusso",
	"吐出量",
	"Fluxo",
	"Flow",
	"Akis",
	"흐름",
	"Поток",
};

const char MMSG_CONTROL[12][20] PROGMEM = {
	"Control",
	"控制界面",
	"Contrôler",
	"Einstellungen",
	"Control",
	"Controllo",
	"制御",			
	"Controlo",
	"Control",
	"Kontrol",
	"제어",		
	"Настройки",
};

const char MMSG_LCD_ON[12][20] PROGMEM = {
	"On",
	"开",
	"Marche",
	"an",
	"Encender",
	"On",
	"オン",
	"Encender",
	"Aan",
	"Açık",
	"에",
	"Вкл",
};

const char MMSG_LCD_OFF[12][20] PROGMEM = {
	"Off",
	"关",
	"Arrêt",
	"aus",
	"Apagar",
	"Off",
	"オフ",
	"Apagar",
	"Uit",
	"Kapalı",
	"끄다",
	"Выкл",
};

const char MMSG_TEMPERATURE[12][24] PROGMEM = {
	"Temperature",
	"温度设置",	
	"Température",
	"Temperatur",
	"Temperatura",
	"Temperatura",
	"温度",
	"Temperatura",
	"Temperatuur",
	"Sicaklik",
	"온도",
	"Температура",
};

const char MMSG_RESTORE_DEFAULTS[12][38] PROGMEM = {
	"Restore Defaults",
	"恢复出厂设置",
	"Restaurer défauts",
	"Standardwerte laden",
	"Rest. fábrica",
	"Ripristina imp.",
	"設定リセット",
	"Rest. de emergen.",
	"Herstel standaard",
	"Varsayilana dondur",
	"설정 되돌리기",	
	"Восстановить настр.",
};

const char MMSG_INFO_SCREEN [12][41] PROGMEM = {	
	"Info screen",
	"信息界面",
	"Surveiller",
	"Info",
	"Pantalla de Inf.",
	"Schermata info",
	"情報画面",
	"Monitorizar",
	"Info scherm",
	"Bilgi Ekrani",
	"처음으로",		
	"Информационный экран",
};

const char MMSG_PREPARE[12][30] PROGMEM = {
	"Prepare",
	"准备界面",
	"Préparer",
	"Vorbereitung",
	"Preparar",
	"Prepara",
	"準備設定",
	"Preparar",
	"Voorbereiden",
	"Hazirlik",
	"준비하기",		// ?? ??
	"Подготовить",
};

const char MMSG_TUNE[12][20] PROGMEM = {
	"Tune",
	"调整打印参数",
	"Régler",
	"Justierung",
	"Ajustar",
	"Regola",
	"調整",
	"Afinar",
	"Afstellen",
	"Ayar",
	"준비하기",		
	"Настроить",
};

const char MMSG_BUTTON_NEXT[12][20] PROGMEM = {
	"Next",
	"继续",
	"Suivant",
	"Weiter",
	"Siguinte",
	"Prossimo",
	"次へ",
	"Proximo",
	"Volgende",
	"Sonraki",
	"다음",		
	"следующий",
};

const char MMSG_BUTTON_PRINT[12][16] PROGMEM = {
	"Print",
	"打印",
	"Imprimer",
	"Drucken",
	"Imprimir",
	"Stampa",
	"プリント",
	"Impressao",
	"het drukken",
	"Baski",
	"인쇄",
	"печать",
};

const char MMSG_PAUSE_PRINT[12][25] PROGMEM = {
	"Pause Print",
	"暂停打印",
	"Pause impression",
	"TF-Druck pausieren",
	"Pausar impresión",
	"Pausa stampa",
	"一時停止",
	"Pausar impressao",
	"Print pauzeren",
	"Duraklat",
	"일시정지",
	"Пауза печати",
};

const char MMSG_RESUME_PRINT[12][35] PROGMEM = {
	"Resume Print",
	"继续打印",
	"Reprendre impr.",
	"TF-Druck fortsetzen",
	"Reanudar impresión",
	"Riprendi stampa",
	"プリント再開",
	"Retomar impressao",
	"Print hervatten",
	"Surdur",
	"재시작",		
	"Продолжить печать",
};

const char MMSG_STOP_PRINT[12][35] PROGMEM = {
	"Stop Print",
	"停止打印",
	"Arrêter impr.",
	"TF-Druck abbrechen",
	"Detener impresión",
	"Arresta stampa",
	"プリント停止",
	"Parar impressao",
	"Print stoppen",
	"Durdur",
	"출력중지",
	"Остановить печать",
};

const char MMSG_PRINT_PAUSED[12][49] PROGMEM = {
	"Print Paused",
	"打印暂停",
	"Impr. en pause",
	"Druck pausiert",
	"Impresión Pausada",
	"Stampa sospesa",
	"プリントが一時停止",
	"Imprimir pausado",
	"Print onderbroken",
	"Baski duraklatildi",
	"일시 정지됨",
	"Печать на паузе",
};

const char MMSG_PRINTING[12][25] PROGMEM = {
	"Printing...",
	"打印...",
	"Impression",
	"Druckt...",
	"Imprimiendo...",
	"Stampa...",
	"プリント中...",
	"Impressao...",
	"het drukken ...",
	"Baski...",
	"출력중...",		
	"Печать...",
};

const char MMSG_FILAMENTLOAD[12][37] PROGMEM = {
	"Load Filament",
	"加载料丝",
	"Charger filament",
	"Filament laden",
	"Cargar filamento",
	"Carica filamento",
	"フィラメントイン",
	"Alimente Filament",
	"Auto Feed Filament",
	"Yem Filamenti",
	"필라멘트 로드",
	"Загрузка филамента",
};

const char MMSG_FILAMENTOUT[12][36] PROGMEM = {
	"Unload Filament",
	"卸载料丝",
	"Retrait filament",
	"Filament entladen",
	"Descargar filamento",
	"Rimuovi filamento",
	"フィラメントインアウト",
	"Retrair Filamento",
	"Auto Retract Filament",
	"Filamenti Geri Cek",
	"필라멘트 언로드",		
	"Авто Втягивание",
};

const char MMSG_ZPROBE_ZOFFSET[12][22] PROGMEM = {
	"Probe Z Offset",
	"设置Z轴偏置",
	"Décalage Z",
	"Sondenversatz Z",
	"Desfase Z",
	"Offset Z sonda",
	"Zオフセット",
	"Desvio Z",
	"Z Offset",
	"Z Offset",
	"프로브 Z 오프셋",	
	"Смещение Z",
};

const char MMSG_NOZZLE_HEATING_FAILED_LCD[12][41] PROGMEM = {
	"Nozzle Heating Failed",
	"喷头加热失败",
	"Buse: Err de chauffe",
	"Duse:HEIZEN ERFOLGLOS",
	"Boquilla:Calent. fallido",
	"Ugello:Riscald. Fallito",
	"ノズル加熱失敗",
	"Bocal:Aquecimento falhou",
	"Nozzle:Voorverw. fout",
	"Nozul:Isınma başarısız",
	"노즐 가열 실패",
	"Сбой нагрева форсунки",
};

const char MMSG_BED_HEATING_FAILED_LCD[12][54] PROGMEM = {
	"Bed Heating Failed",
	"平台加热失败",
	"Lit: Err de chauffe",
	"Bett:HEIZEN ERFOLGLOS",
	"Cama:Calent. fallido",
	"Piatto:Riscald. Fallito",
	"ベッド加熱失敗",
	"Base:Aquecimento falhou",
	"Bed:Voorverw. fout",
	"Tabla:Isınma başarısız",
	"침대 난방 실패",
	"Отопление кровати не удалось",
};

const char MMSG_ERR_BED_TEMP[12][41] PROGMEM = {
	"BED TEMP ERROR",
	"平台温度错误",
	"ERREUR TEMP. LIT",
	"BETTTEMPERATURFEHLER",
	"ERROR DE TEMP. DE LA CAMA",
	"ERRORE TEMP LETTO",
	"ベッド温度エラー",
	"ERRO DE TEMP",
	"BED TEMP FOUT",
	"YATAK ISI HATASI",
	"침대 온도 오류.",
	"ОШИБКА ТЕМП.",
};

const char MMSG_ERR_NOZZLE_TEMP[12][47] PROGMEM = {
	"Nozzle TEMP ERROR",
	"喷头温度错误",
	"ERREUR TEMP Buse",
	"Düsentemperaturfehler",
	"ERROR DE TEMPERATURA",
	"ERRORE TEMPERATURA Ugello",
	"ノズル温度エラー",
	"ERRO DE TEMP",
	"Mondstuk TEMP FOUT",
	"Nozul SICAKLIK HATASI",
	"노즐 온도 오류",
	"ОШИБКА ТЕМПЕРАТУРЫ сопла",
};

const char MMSG_HALTED[12][40] PROGMEM = {
	"PRINTER HALTED",
	"打印停机",
	"IMPR. STOPPÉE",
	"DRUCKER GESTOPPT",
	"IMPRESORA DETENIDA",
	"STAMPANTE FERMATA",
	"プリンタは停止しました",
	"IMPRESORA DETENIDA",
	"PRINTER GESTOPT",
	"YAZICI DURDURULDU",
	"프린터 정지",
	"ПРИНТЕР ОСТАНОВЛЕН",
};

const char MMSG_PLEASE_RESET[12][31] PROGMEM = {
	"Please Reset",
	"请重启打印机",
	"Redémarrer SVP",
	"Bitte neustarten",
	"Por favor, reinicie",
	"Riavviare prego",
	"リセットしてください",
	"Por favor, reinicie",
	"Reset A.U.B.",
	"Lütfen Resetleyin",
	"재설정하십시오",
	"Сделайте сброс",
};


const char MMSG_HEATING[12][20] PROGMEM = {
	"Heating...",
	"加热中...",
	"en chauffe...",
	"heizt...",
	"Calentando...",
	"Riscaldamento...",
	"加熱中...",
	"Aquecendo...",
	"Voorwarmen...",
	"Isıtma...",
	"난방...",
	"Нагрев...",
};

const char MMSG_COOLING[12][26] PROGMEM = {
	"Cooling...",
	"冷却中...",
	"Refroidissement",
	"kühlt...",
	"Enfriando...",
	"Raffreddamento...",
	"加熱停止...",
	"Enfriando...",
	"Koeling...",
	"Soğutma...",
	"냉각...",
	"Охлаждение...",
};

const char MMSG_BED_HEATING[12][29] PROGMEM = {
	"Bed Heating...",
	"加热中...",
	"Lit en chauffe...",
	"Bett heizt...",
	"Calentando Cama...",
	"Risc. piatto...",
	"ベッド 加熱中...",
	"Aquecendo base...",
	"Bed voorverw...",
	"Isıtma...",
	"난방...",
	"Нагрев стола...",
};

const char MMSG_BED_COOLING[12][35] PROGMEM = {
	"Bed Cooling...",
	"冷却中...",
	"Refroid. du lit...",
	"Bett kühlt...",
	"Enfriando Cama...",
	"Raffr. piatto...",
	"ベッド加熱停止...",
	"Enfriando Cama...",
	"Koeling...",
	"Soğutma...",
	"냉각...",
	"Охлаждение стола...",
};

const char MMSG_MACHINE_INFO[12][31] PROGMEM = {
	"Printer Info",
	"打印机信息",
	"Infos imprimante",
	"Drucker-Info",
	"Info. Impresora",
	"Info. stampante",
	"プリンター情報",
	"Em formacao",
	"Printer Info",
	"Makine Bilgisi",
	"프린터 정보",
	"Данные принтера",
};

const char MMSG_CARD_MENU[12][31] PROGMEM = {
	"Print from TF",
	"TF卡界面",
	"Impression TF",
	"Druck vom TF",
	"Imprim. desde TF",
	"Stampa da TF",
	"TFからプリント",
	"Imprimir do TF",
	"Print van TF",
	"TF den Yazdir",
	"TF에서 인쇄",		
	"Печать с TF карты",
};

const char MMSG_SD_INSERTED[12][46] PROGMEM = {
	"TF inserted",
	"TF卡已插入",
	"TF inséré",
	"TF erkannt",
	"TF insertado",
	"TF inserito",
	"TFが挿入されました",
	"Cartão inserido",
	"Kaart ingestoken",
	"TF Yerlesti.",
	"TF 삽입",
	"Карта вставлена",
};

const char MMSG_SD_REMOVED[12][31] PROGMEM = {
	"TF removed",
	"TF卡已拔出",
	"TF retiré",
	"TF entfernt",
	"TF retirado",
	"TF rimosso",
	"TFがありません",
	"Cartão removido",
	"Kaart verwijderd",
	"TF cikarildi.",
	"제거된 TF",
	"Карта извлечена",
};

const char MMSG_NO_CARD[12][31] PROGMEM = {
	"No TF card",
	"无卡",
	"Pas de TF",
	"Keine TF",
	"TF no presente",
	"TF non presente",
	"TFがありません",
	"Sem cartão TF",
	"Geen TF kaart",
	"TF Kart Yok",
	"TF 카드 없음",
	"Нет TF карты",
};

const char MMSG_WELCOME_MENU[12][24] PROGMEM = {
	"Wizard",
	"使用向导",
	"Lancer la config.",
	"Magier",
	"Wizzard",
	"procedura guidata",
	"ウィザード",
	"Usando o assistente",
	"De wizard gebruiken",
	"Sihirbazi kullanma",
	"마법사",	
	"колдун",
};


const char MMSG_NETWORK_MENU[12][26] PROGMEM = {
	"Wifi Network",
	"Wifi网络设置",		
	"Reseau Wifi",
	"WiFi-Netzwerk",
	"red Wifi",
	"Rete Wifi",
	"Wifiネットワーク",
	"Rede Wi-Fi",
	"Wifi netwerk",
	"Wifi Agi",
	"와이파이 네트워크",
	"Сеть Wi-Fi",
};

const char MMSG_NETWORK_INFO[12][30] PROGMEM = {
	"Wifi Info",
	"显示Wifi信息",	
	"Info appareil",
	"WLAN-Gerateinfo",
	"dispositivo wifi",
	"informazioni wifi",
	"Wifi情報",
	"Informacao Wifi",
	"Wifi apparaatinfo",
	"Wifi Cihaz Bilgisi",
	"와이파이 정보",
	"Информация о Wifi",
};

const char MMSG_NETWORK_QRCODE[12][30] PROGMEM = {
	"Wifi QR Code",
	"显示设备二维码",
	"Code QR Wi-Fi",
	"WLAN-QR-Code",
	"Codigo QR Wi-Fi",
	"Codice QR Wi-Fi",
	"WifiQRコード",
	"Codigo QR Wi-Fi",
	"Wifi QR-code",
	"Wi-Fi QR Kodu",
	"와이파이 QR 코드",	
	"QR-код Wi-Fi",
};

const char MMSG_NETWORK_RESET[12][30] PROGMEM = {
	"Wifi Reset",
	"复位wifi设置",	
	"Wi-Fi Reinitialiser",
	"WLAN-Reset",
	"Reiniciar Wifi",
	"Ripristino Wi-Fi",
	"Wifiリセット",
	"Redefinicao de Wi-Fi",
	"Wifi resetten",
	"Wifi Sifirlama",
	"와이파이 재설정",
	"Сброс Wi-Fi",
};

const char MMSG_NETWORK_RESET_LINE1[12][35] PROGMEM = {
	"Wifi has been reset",
	"wifi设置已复位",	
	"Le Wi-Fi a ete",
	"WLAN wurde",
	"Se ha reiniciado el",
	"Il Wi-Fi e stato",
	"Wifiがリセットされました",
	"O Wi-Fi foi redefinido",
	"Wifi is gereset",
	"Wifi sifirlandi",
	"Wi-Fi가 재설정되었습니다",
	"Wi-Fi был сброшен",
};

const char MMSG_NETWORK_RESET_LINE2[12][22] PROGMEM = {
	"",
	"",
	"reinitialise",
	"zuruckgesetzt",
	"wifi",
	"ripristinato",
	"",
	"",
	"",
	"",
	"",
	"",
};

const char MMSG_NETWORK_ERROR[12][30] PROGMEM = {
	"Wifi Device Error",
	"wifi设备故障",	
	"Erreur Wi-Fi",
	"WLAN-Fehler",
	"error wifi",
	"Errore Wi-Fi",
	"Wifiデバイスエラー",
	"Erro de Wi-Fi",
	"Wifi-apparaatfout",
	"Wifi Aygit Hatasi",
	"와이파이 기기 오류",		
	"Ошибка Wi-Fi",
};

const char MMSG_NETWORK_OTA[12][30] PROGMEM = {
	"Wifi OTA Update",
	"Wifi设备OTA",// Wifi设备OTA 		
	"Mise a jour Wifi OTA",
	"Wifi OTA Update",
	"dispositivo wifi ota",
	"Aggiornamento OTA",
	"WifiOTAアップデート",
	"Atualizacao OTA",
	"Wifi OTA Update",
	"OTA Guncellemesi",
	"와이파이 OTA 업데이트",
	"OTA Обновление",
};

const char MMSG_NETWORK_OTA_MENU[12][36] PROGMEM = {
	"OTA UPDATING...",
	"正在更新...",
	"MISE A JOUR OTA ...",
	"OTA AKTUALISIEREN ...",
	"OTA ACTUALIZANDO ...",
	"AGGIORNAMENTO OTA ...",
	"OTA アップデート中...",
	"OTA ATUALIZANDO ...",
	"OTA更新...",
	"OTA GüNCELLEME ...",
	"오타 업데이트 중...",
	"ОТА ОБНОВЛЕНИЕ ...",
};

const char MMSG_NETWORK_OTA_1[12][44] PROGMEM = {
	"Don't turn off power",
	"不要关闭电源",			// 不要关闭电源		
	"ne pas eteindre",
	"Macht weiter so",
	"No apague el poder",
	"continua ad accendere",
	"電源を切らないでください",
	"Nao desligue a energ-",
	"Niet uitschakelen uit",
	"Gucu kapatmayin",
	"전원을 끄지 마십시오",	
	"Подожди минуту",
};

const char MMSG_NETWORK_OTA_2[12][50] PROGMEM = {
	"Wait a minute",
	" ",	
	"Attends une minute",
	"Warte eine Minute",
	"Espera un minuto",
	"Apetta un minuto",
	"暫くお待ちください",
	"ia e espere um minuto",
	"en wacht een minuut",
	"Bir dakika bekle",
	"잠깐 기다려요",
	" ",
};

const char MMSG_UPLOADING_1[12][30] PROGMEM = {
	"Downloading:",
	"正在下载",			// 正在下载	
	"Telechargement:",
	"Wird heruntergeladen:",
	"Descargando:",
	"Download:",
	"ダウンロード中:",
	"Baixando:",
	"Downloaden:",
	"Indiriyor:",
	"다운로드 중:",
	"Загрузка:",
};

const char MMSG_UPLOADING_FAIL[12][32] PROGMEM = {
	"Download Fail",
	"下载失败",			// 下载失败		
	"Telecharger Fail",
	"Scheitern",
	"Error de descarga",
	"Scarica Fail",
	"ダウンロード失敗",
	"Falha no download",
	"Downloaden mislukt",
	"Indiriyor:",
	"다운로드 실패",	
	"Скачать Fail",
};

const char MMSG_UPLOADING_OK[12][36] PROGMEM = {
	"Download OK",
	"下载完成",
	"Telecharger OK",
	"Herunterladen Okay",
	"Descargar Aceptar",
	"Scarica OK",
	"ダウンロード完了",
	"Baixar OK",
	"Download OK",
	"Indir Tamam",
	"다운로드 확인",
	"Скачать ОК",
};

const char MMSG_LANGUAGE_MENU[12][26] PROGMEM = {
	"Language",
	"语言设置",		
	"La langue",
	"Sprache",
	"Idioma",
	"linguaggio",
	"言語",
	"Lingua",
	"Taal",
	"Dil",
	"언어",	
	"формулировки",
};

const char MMSG_LANGUAGE_EN[12][22] PROGMEM = {
	"English",
	"英语",		
	"Anglais",
	"Englisch",
	"Ingles",
	"Inglese",
	"英語",
	"Ingles",
	"Engels",
	"Ingilizce",
	"영어",
	"английский",
};

const char MMSG_LANGUAGE_CN[12][20] PROGMEM = {
	"Chinese",
	"中文",	
	"Chinoise",
	"Chinesisch",
	"Chino",
	"Cinese",
	"中国語",
	"Chines",
	"Chinese",
	"Cince",
	"중국인",
	"Китайский",
};

const char MMSG_LANGUAGE_FR[12][24] PROGMEM = {
	"French",
	"法语",	
	"Francais",
	"Franzosisch",
	"Frances",
	"Francese",
	"フランス語",
	"Frances",
	"Frans",
	"Fransizca",
	"프랑스 국민",
	"Французский",
};

const char MMSG_LANGUAGE_DE[12][20] PROGMEM = {
	"German",
	"德语",		
	"allemand",
	"Deutsche",
	"Aleman",
	"Tedesca",
	"ドイツ語",
	"Alemao",
	"Duits",
	"Almanca",
	"독일 사람",
	"Немецкий",
};

const char MMSG_LANGUAGE_ES[12][20] PROGMEM = {
	"Spanish",
	"西班牙语",		
	"Espagnol",
	"Spanisch",
	"Espanol",
	"Spagnola",
	"スペイン語",
	"Espanhol",
	"Spaans",
	"Ispanyol",
	"스페인의",
	"испанский",
};

const char MMSG_LANGUAGE_IT[12][20] PROGMEM = {
	"Italian",
	"意大利语",	
	"Italienne",
	"Italienisch",
	"Italiano",
	"Italiana",
	"イタリア語",
	"Italiano",
	"Italiaans",
	"Italyan",
	"이탈리아 사람",
	"итальянец",
};

const char MMSG_LANGUAGE_KANA[12][20] PROGMEM = {
	"Japanese",
	"日语",
	"Japonais",
	"Japanisch",
	"Japonesa",
	"Giapponese",
	"日本語",
	"Japones",
	"Japans",
	"Japonca",
	"일본어",
	"Японский",
};

const char MMSG_LANGUAGE_PT[12][22] PROGMEM = {
	"Portugal",
	"葡萄牙语",
	"le Portugal",
	"Portugal",
	"Portugues",
	"Portogallo",
	"ポルトガル語",
	"Portugal",
	"Portugal",
	"Portekiz",
	"포르투갈",
	"Португалия",
};

const char MMSG_LANGUAGE_NL[12][24] PROGMEM = {
	"Dutch",
	"荷兰语",
	"neerlandais",
	"Niederlandisch",
	"Holandes",
	"Olandese",
	"オランダ語",
	"Holandes",
	"Nederlands",
	"Flemenkce",
	"네덜란드 사람",
	"Голландский",
};

const char MMSG_LANGUAGE_RU[12][20] PROGMEM = {
	"Russian",
	"俄语",
	"russe",
	"Russisch",
	"Ruso",
	"Russo",
	"ロシア語",
	"Russo",
	"Russisch",
	"Rusca",
	"러시아인",
	"русский",
};

const char MMSG_LANGUAGE_TR[12][20] PROGMEM = {
	"Turkish",
	"土耳其语",
	"Turc",
	"Turkisch",
	"Turco",
	"Turco",
	"トルコ語",
	"Turco",
	"Turks",
	"Turk",
	"터키어",
	"турецкий",
};

const char MMSG_LANGUAGE_KR[12][20] PROGMEM = {
	"Korean",
	"韩语",
	"Coreen",
	"Koreanisch",
	"Coreano",
	"Coreano",
	"韓国語",
	"Coreano",
	"Koreaans",
	"Koreli",
	"한국인",
	"Корейский",
};



const char MMSG_INFO_MACHINE[12][20] PROGMEM = {
	"Machine: ",
	"设备: ",
	"Machine: ",
	"Maschine: ",
	"Maquina: ",
	"Macchina: ",
	"マシン: ",
	"Maquina: ",
	"Machine: ",
	"Makine: ",
	"기계: ",
	"Машина: ",
};

const char MMSG_INFO_HARDWARE[12][28] PROGMEM = {
	"Hardware: ",
	"硬件型号: ",
	"Materiel: ",
	"Hardware: ",
	"Hardware: ",
	"Hardware: ",
	"ハードウェア: ",
	"Hardware: ",
	"Sprzet: ",
	"Donanim: ",
	"하드웨어: ",
	"Оборудование:",
};

const char MMSG_INFO_VERSION[12][40] PROGMEM = {
	"Version: ",
	"固件版本: ",
	"Version: ",
	"Ausfuhrung: ",
	"Version: ",
	"Versione: ",
	"バージョン: ",
	"Versao: ",
	"Versie: ",
	"Surum: ",
	"버전: ",
	"Версия: ",
};



const char MMSG_WELCOME_SCREEN_1[12][25] PROGMEM = {
	"Welcome",
	"欢迎",
	"Bienvenue",
	"Willkommen",
	"Bienvenido",
	"Benvenuto",
	"いらっしゃいませ",
	"Bem vinda",
	"Welkom",
	"Hosgeldiniz",
	"어서 오십시오",
	"желанный",
};

const char MMSG_WELCOME_SCREEN_2[12][41] PROGMEM = {
	"STEP1:LOAD FILAMENT",
	"第1步 载入料丝",			// 第1步 载入料丝	
	"1 Charger le filament",
	"1 FILAMENT LADEN",
	"1 cargar filamento",
	"1 FILAMENTO DI CARICO",
	"1 フィラメントイン",
	"1 Filamento de Carga",
	"1 LAAD FILAMENT",
	"1 YUK FILAMENTI",
	"1단계: 필라멘트 로드",	
	"1 НАГРУЗКА FILAMENT",
};

const char MMSG_WELCOME_SCREEN_3[12][44] PROGMEM = {
	"STEP2:INSERT TF CARD",
	"第2步 插入TF卡",
	"2 INSERT CARTE TF",
	"2 ERKANNT TF-KARTE",
	"2 Insertar tarjeta TF",
	"2 INSERITA TF CARD",
	"2：TFガソウニュウシテクダサイ",
	"2 Inserir cartao TF",
	"2 PLAATS TF-KAART",
	"2 INSERT TF KART",
	"2단계: TF 카드 삽입",
	"2 Вставьте TF-карту",
};

const char MMSG_WELCOME_SCREEN_4[12][43] PROGMEM = {
	"STEP3:CHOICE FILE",
	"第3步 打印SD卡文件",
	"3 CHOIX FICHIER",
	"3 WAHLDATEI",
	"3 Archivo de eleccion",
	"3 FILE DI SCELTA",
	"3 SDからプリント",
	"3 Arquivo de escolha",
	"3 KEUZEBESTAND",
	"3 SECIM DOSYASI",
	"3단계: SD 카드 파일 인쇄",	
	"3 ВЫБОР ФАЙЛА",
};



const char MMSG_WELCOME_SKIP[12][20] PROGMEM = {
	"Quit",
	"退出",
	"Quitter",
	"Verlassen",
	"Salir",
	"Smettere",
	"終了する",
	"Sair",
	"Ophouden",
	"cikmak",
	"그만두다",
	"Уволиться",
};

const char MMSG_FILAMENT_CHANGE_INIT_1[12][34] PROGMEM = {
	"Wait for start",
	"等待开始",		
	"Attendez debut",
	"Warte auf den",
	"Espere a que comience",
	"Attendere avvio",
	"暫くお待ちください",
	"Aguarde o inicio",
	"Wacht voor start",
	"Baslama bekleniyor",
	"시작을 기다리다",
	"Дождитесь",
};

const char MMSG_FILAMENT_CHANGE_INIT_2[12][36] PROGMEM = {
	"of the filament",
	"料丝",
	"du filament",
	"Start des",
	"el cambio",
	"del cambio",
	"フィラメント",
	"da mudanca",
	"filament te",
	"filamanin",
	"필라멘트",
	"начала",
};

const char MMSG_FILAMENT_CHANGE_INIT_3[12][32] PROGMEM = {
	"change",
	"操作",
	"changement",
	"Filamentwechsels",
	"de filamento",
	"di filamento",
	"交換を開始します",
	"de filamento",
	"verwisselen",
	"degisimi",
	"변화",	
	"смены нити",
};

const char MMSG_FILAMENT_CHANGE_UNLOAD_1[12][36] PROGMEM = {
	"Wait for",
	"等待",
	"Attendez ",
	"Warte auf",
	"Espere a que",
	"Attendere",
	"暫くお待ちください",
	"Esperar para",
	"Wacht voor",
	"Bekleniyor",
	"기다립니다",
	"Дождитесь",
};

const char MMSG_FILAMENT_CHANGE_UNLOAD_2[12][42] PROGMEM = {
	"filament unload",
	"卸载料丝",	
	"dechargement",
	"Herausnahme",
	"se descargue",
	"l'espulsione",
	"フィラメント抜き出し中",
	"descarregar",
	"filament uit",
	"filamanin cikmasi",
	"필라멘트 언로드",
	"выгрузки",
};

const char MMSG_FILAMENT_CHANGE_UNLOAD_3[12][20] PROGMEM = {
	" ",
	" ",
	"du filament",
	"des Filaments...",
	"el filamento",
	"del filamento",
	" ",
	"filamento",
	"te laden",
	" ",
	" ",
	"нити",
};

const char MMSG_FILAMENT_CHANGE_INSERT_1[12][42] PROGMEM = {
	"Insert filament",
	"插入料丝",
	"Inserez le filament",
	"Filament einlegen",
	"Inserte filamento",
	"Inserisci il",
	"フィラメントを挿入し",
	"Insira filamento e",
	"Laad filament",
	"Filamani yukle",
	"필라멘트 삽입",
	"Вставьте нить",
};

const char MMSG_FILAMENT_CHANGE_INSERT_2[12][44] PROGMEM = {
	"and press button",
	"并按键",
	"et presser le bouton",
	"und Knopf",
	"y presione el boton",
	"filamento e",
	"クリックすると",
	"pressione o botao",
	"en druk knop",
	"ve devam icin",
	"그리고 버튼을 눌러",
	"и нажмите",
};

const char MMSG_FILAMENT_CHANGE_INSERT_3[12][24] PROGMEM = {
	"to continue...",
	" ",	
	"pour continuer...",
	"drucken...",
	"para continuar...",
	"premi per cont",
	"続行します",
	"para continuar...",
	"om verder...",
	"tusa bas...",
	"계속하다",
	"кнопку",
};

const char MMSG_FILAMENT_CHANGE_HEAT_1[12][44] PROGMEM = {
	"Press button to",
	"按键开始",		
	"Presser le bouton...",
	"Knopf drucken um",
	"Presione él boton para",
	"Premi per",
	"クリックすると続行して",
	"Pressione o botao",
	"Klik knop om...",
	"Nozulu Isitmak icin",
	"버튼을 눌러",
	"Нажмите кнопку,",
};

const char MMSG_FILAMENT_CHANGE_HEAT_2[12][27] PROGMEM = {
	"heat nozzle.",
	"预热喷头",
	"pour chauffer la buse",
	"Duse aufzuheizen.",
	"Calentar la boquilla",
	"riscald ugello.",
	"予熱ノズル",
	"para aquecer o bocal",
	"verw. nozzle.",
	"Butona Bas.",
	"열 노즐.",	
	"чтобы нагреть",
};

const char MMSG_FILAMENT_CHANGE_HEATING_1[12][31] PROGMEM = {
	"Heating nozzle",
	"预热喷头",
	"Chauffage des buse",
	"Duse heizt auf...",
	"Calentando boquilla",
	"Riscald. ugello",
	"ノズル予熱中",
	"Bico de aquecimento",
	"Nozzle verw.",
	"Nozul Isiniyor",
	"가열 노즐",	
	"Отопление сопло",
};

const char MMSG_FILAMENT_CHANGE_HEATING_2[12][38] PROGMEM = {
	"Please wait...",
	"等待...",
	"Attendez, SVP...",
	"Bitte warten...",
	"Espere por favor",
	"Attendere...",
	"暫くお待ちください...",
	"Por favor, espere",
	"Wacht a.u.b.",
	"Lutfen Bekleyin...",
	"기다리세요...",	
	"Подождите...",
};

const char MMSG_FILAMENT_CHANGE_LOAD_1[12][36] PROGMEM = {
	"Wait for",
	"等待",
	"Attendre la charge",
	"Warte auf",
	"Esperar la carga",
	"Attendere",
	"暫くお待ちください",
	"Aguarde a carga",
	"Wacht voor",
	"Bekleniyor",
	"기다립니다",
	"Дождитесь",
};

const char MMSG_FILAMENT_CHANGE_LOAD_2[12][32] PROGMEM = {
	"filament load",
	"载入料丝",	
	"du filament",
	"Laden des",
	"de filamento",
	"il caricamento",
	"フィラメント装填",
	"de filamento",
	"filament te",
	"filamanin yuklenmesi",
	"필라멘트 부하",	
	"загрузки нити",
};

const char MMSG_FILAMENT_CHANGE_LOAD_3[12][20] PROGMEM = {
	" ",
	" ",
	" ",
	" ",
	" ",
	"del filamento",
	" ",
	" ",
	"laden",
	" ",
	" ",
	" ",
};

const char MMSG_FILAMENT_CHANGE_PURGE_1[12][28] PROGMEM = {
	"Wait for",
	" ",
	"Attendez la purge",
	"Warte auf",
	"Esperar la purga",
	"Attendere",
	"暫くお待ちください",
	"Aguarde a limpeza",
	"Wacht op",
	"Filament",
	"기다립니다",
	"Дождитесь",
};

const char MMSG_FILAMENT_CHANGE_PURGE_2[12][28] PROGMEM = {
	"filament purge",
	"等待载入",
	"du filament",
	"Entleeren des",
	"del filamento",
	"lo spurgo",
	"フィラメントパージ",
	"do filamento",
	"filamentreiniging",
	"temizligini",
	"필라멘트 퍼지",
	"чистки нити",
};

const char MMSG_FILAMENT_CHANGE_PURGE_3[12][20] PROGMEM = {
	" ",
	" ",
	" ",
	"Filaments",
	" ",
	"del filamento",
	" ",
	" ",
	" ",
	"bekleyin",
	" ",
	" ",
};

const char MMSG_FILAMENT_CHANGE_RESUME_1[12][38] PROGMEM = {
	"Wait for print",
	"等待",
	"Attendez que l'impression",
	"Warte auf",
	"Esperando que se reanude",
	"Attendere",
	"暫くお待ちください",
	"Aguarde a impressao",
	"Wacht voor print",
	"Baskinin",
	"인쇄 대기",
	"Подождите, пока",
};

const char MMSG_FILAMENT_CHANGE_RESUME_2[12][36] PROGMEM = {
	"to resume",
	"继续打印",
	"pour reprendre",
	"Fortsetzung des",
	"la impresion",
	"la ripresa",
	"プリントを再開します",
	"ser retomada",
	"om verder",
	"surdurulmesini",
	"재개하다",	
	"печать на",
};

const char MMSG_FILAMENT_CHANGE_RESUME_3[12][20] PROGMEM = {
	" ",
	" ",
	" ",
	"Druckes...",
	" ",
	"della stampa",
	" ",
	" ",
	"te gaan",
	"bekle",
	" ",
	"резюме",
};

const char MMSG_ZPROBE_TITLE[12][26] PROGMEM = {
	"Adjust Z Offset(mm)",
	"Z轴偏置(mm)",
	"Ajuster Z Offset (mm)",
	"Z-Versatz (mm)",
	"Ajuste Z Offset(mm)",
	"Offset Z (mm)",
	"Zオフセット(mm)",
	"Ajustar Z offset(mm)",
	"Passen Z Offset (mm)",
	"Z Ofset (mm) ayarla",
	"Z 오프셋 조정(mm)",
	"Смещение Z(mm)",
};

const char MMSG_FILAMENT_CHANGE_HEADER_PAUSE[12][50] PROGMEM = {
	"PRINT PAUSED",
	"打印暂停",		
	"IMPRIMER PAUSED",
	"DRUCK PAUSIERT",
	"IMPR. PAUSADA",
	"STAMPA IN PAUSA",
	"プリントが一時停止",
	"IMPRIMIR PAUSADO",
	"AFDRUK GEPAUZEERD",
	"YAZDIR DURDU",
	"인쇄 일시 중지",
	"Печать на паузе",
};

const char MMSG_FILAMENT_CHANGE_OPTION_HEADER[12][26] PROGMEM = {
	"RESUME OPTIONS:",
	"继续操作",
	"OPTIONS DE REPRISE:",
	"FORTS. OPTIONEN:",
	"OPC. REINICIO:",
	"OPZIONI:",
	"プリント再開:",
	"OPCOES DE RESUMO:",
	"RESUME OPTIES:",
	"DEVAM SECENEKLERI:",
	"이력서 옵션:",	
	"ПАРАМЕТРЫ:",
};

const char MMSG_FILAMENT_CHANGE_OPTION_PURGE[12][31] PROGMEM = {
	"Purge more",
	"载入更多料丝",
	"purge plus",
	"Mehr entleeren",
	"purgar mas",
	"Libera di piu",
	"もっとパージ",
	"Purgar mais",
	"Purge meer",
	"daha tasfiye et",
	"더 퍼지",
	"Очистить больше",
};

const char MMSG_FILAMENT_CHANGE_OPTION_RESUME[12][22] PROGMEM = {
	"Continue",
	"继续",
	"Continuer",
	"Drucke weiter",
	"Seguir",
	"Riprendi stampa",
	"続行します",
	"Retomar impressao",
	"Hervat print",
	"Baskiyi surdur",
	"계속하다",
	"Продолжить",
};

const char MMSG_FILAMENT_CHANGE_NOZZLE[12][24] PROGMEM = {
	"  Nozzle: ",
	" 喷头温度:",		
	"  Buse: ",
	"  Duse: ",
	" Boquilla:",
	"  Ugello: ",
	"  ノズル: ",
	"  Bocal:  ",
	"  Nozzle: ",
	"  Nozul: ",
	"  대통 주둥이: ",	
	"Насадка:",
};

const char MMSG_ERR_X_HOMING_FAILED[12][44] PROGMEM = {
	"X Homing failed",
	"X轴归位失败",
	"X Erreur de retour",
	"X Homing gescheitert",
	"X homing fallo",
	"X Home fallito",
	"X軸原点復帰失敗",
	"X Homing falhou",
	"X Homing is mislukt",
	"X Homing basarisiz oldu",
	"X \x8f\xe5 \xd0\xd1\xec \xbe\xf5",// 노즐 원위치 실패
	"X не удалось",
};

const char MMSG_ERR_Y_HOMING_FAILED[12][44] PROGMEM = {
	"Y Homing failed",
	"Y轴归位失败",
	"Y Erreur de retour",
	"Y Homing gescheitert",
	"Y homing fallo",
	"Y Home fallito",
	"Y軸原点復帰失敗",
	"Y Homing falhou",
	"Y Homing is mislukt",
	"Y Homing basarisiz oldu",
	"Y \x8f\xe5 \xd0\xd1\xec \xbe\xf5",// 노즐 원위치 실패
	"Y не удалось",
};

const char MMSG_ERR_Z_HOMING_FAILED[12][44] PROGMEM = {
	"Z Homing failed",
	"Z轴归位失败",		
	"Z Erreur de retour",
	"Z Homing gescheitert",
	"Z homing fallo",
	"Z Home fallito",
	"Z軸原点復帰失敗",
	"Z Homing falhou",
	"Z Homing is mislukt",
	"Z Homing basarisiz oldu",
	"Z \x8f\xe5 \xd0\xd1\xec \xbe\xf5",// 노즐 원위치 실패
	"Z не удалось",
};


const char MMSG_ERR_PROBING_FAILED[12][40] PROGMEM = {
	"Probing failed",
	"自动调平失败",
	"Erreur",
	"Probing gescheitert",
	"el sondeo fallo",
	"Probing fallito",
	"ベッドレベリング失敗",
	"Sondagem falhou",
	"Testen is mislukt",
	"Basarisiz sondalama",
	"프로빙 실패",
	"не удалось",
};




const char MMSG_POWER_LOSS_RECOVERY[12][30] PROGMEM = {
	"Power Loss Recovery",
	"恢复打印",
	"Recup. des pertes",
	"Wiederherstellung",
	"Recuperacion energia",
	"Recupero",
	"プリント再開",
	"Retomar a recuperacao",
	"Herstel stroomverlies",
	"Guc Kaybi Kurtarma",
	"전력 손실 복구",
	"восстановление",
};

const char MMSG_SAVED_OFF[12][48] PROGMEM = {
	"Save Printing and Off",
	"保存关机",
	"Enregistrer et OFF",
	"Speichern und weg",
	"Guardar y apagar",
	"Salva stampa e Off",
	"プリント保存と電源オフ",
	"Salvar e Desativar",
	"Opslaan en Off",
	"Kaydet ve Kapat",
	"인쇄 저장 및 끄기",
	"Сохранить и Off",
};

const char MMSG_SAVED_OFF_1[12][48] PROGMEM = {
	"Wait for printing",
	"等待打印停止后",	
	"Attendez l'arret",
	"Warten fur Druckstopp",
	"Espere a que se",
	"Attendere fermata",
	"プリント停止をお待ち",
	"Aguarde a parada da",
	"Wacht tot het afdru-",
	"Daha sonra, durdurma",
	"인쇄 대기",
	"Дождитесь печати",
};

const char MMSG_SAVED_OFF_2[12][37] PROGMEM = {
	"stop, then turn off",
	"关闭电源",	
	"de l'impression, puis",
	"dann Ausschalten",
	"detenga la impresion,",
	"la stampa,",
	"電源オフ",
	"impressao e desligue",
	"kken te stoppen,",
	"baski bekleyin",
	"중지, 다음 해제",
	"стоп, затем",
};

const char MMSG_SAVED_OFF_3[12][35] PROGMEM = {
	"the power",
	" ",	
	"coupez l'alimentation",
	" ",
	"luego apague",
	"quindi spegnere",
	"パワー",
	"a energia",
	"zet de stroom uit",
	"gucu kapatin",
	"전원",
	"выключите питание",
};


const char MMSG_PRE_PAGE[12][22] PROGMEM = {
	"Previous",
	"上一页",		
	"Precedent",
	"Bisherige",
	"Anterior",
	"Precedente",
	"前のページ",
	"Anterior",
	"voorgaand",
	"Onceki",
	"이전 페이지",
	"Назад",
};

const char MMSG_NEXT_PAGE[12][22] PROGMEM = {
	"Next",
	"下一页",
	"Prochain",
	"Nachster",
	"Proximo",
	"Prossimo",
	"次のページ",
	"Proximo",
	"volgende",
	"Sonraki",
	"다음 페이지",		
	"следующий",
};



const char MMSG_RESTORE_PRINT[12][28] PROGMEM = {
	"Resume printing...",
	"恢复打印...",
	"Reprise...",
	"Druck fortsetzen...",
	"Retomar impresion",
	"Riprendi stampa...",
	"プリント再開...",
	"Curriculo...",
	"Doorgaan met...",
	"Surdurme ...",
	"인쇄 재개...",	
	"Резюмируя ...",
};

const char MMSG_OCTOPRINT_OPEN[]  PROGMEM = {
	"Octoprint Online"
};

const char MMSG_OCTOPRINT_CLOSE[]  PROGMEM = {
	"Octoprint Offline"
};


const char MMSG_INFO_HEIGHT[12][18] PROGMEM = {
	"LayerHeight:",
	"层高:",
	"Precision:",
	"LayerHeight:",
	"LayerHeight:",
	"LayerHeight:",
	"層の厚さ:",
	"LayerHeight:",
	"LayerHeight:",
	"LayerHeight:",
	"레이어 높이:",
	"LayerHeight:",
};

const char MMSG_INFO_DENSITY[12][14] PROGMEM = {
	"Density:",
	"填充率:",
	"Densite:",
	"Density:",
	"Density:",
	"Density:",
	"充填率:",
	"Density:",
	"Density:",
	"Density:",
	"밀도:",
	"Density:",
};

const char MMSG_INFO_FILAMENT[12][30] PROGMEM = {
	"Filament:",
	"料丝类型:",
	"Filament:",
	"Filament:",
	"Filament:",
	"Filament:",
	"材料タイプ:",
	"Filament:",
	"Filament:",
	"Filament:",
	"필라멘트:",
	"Filament:",
};

const char MMSG_INFO_USAGE[12][32] PROGMEM = {
	"Usage:",
	"料丝用量:",
	"Usage:",
	"Usage:",
	"Usage:",
	"Usage:",
	"材料の投与量:",
	"Usage:",
	"Usage:",
	"Usage:",
	"필라멘트 복용량:",
	"Usage:",
};

const char MMSG_INFO_TEMP[12][24] PROGMEM = {
	"Temperature:",
	"打印温度:",
	"Temperature:",
	"Temperature:",
	"Temperature:",
	"Temperature:",
	"プリント温度:",
	"Temperature:",
	"Temperature:",
	"Temperature:",
	"온도:",
	"Temperature:",
};
	
const char MMSG_INFO_TIME[12][20] PROGMEM = {
	"PrintTime:",
	"打印时间:",
	"Temps d'impre:",
	"PrintTime:",
	"PrintTime:",
	"PrintTime:",
	"プリント時間:",
	"PrintTime:",
	"PrintTime:",
	"PrintTime:",
	"인쇄 시간:",
	"PrintTime:",
};

const char MMSG_SETTING_IAP[12][40] PROGMEM = {
	"Firmware Update",
	"更新主板固件",	     // 更新
	"Mettre a jour",
	"Aktualisieren",
	"ctualizacion",
	"Aggiornare",
	"ファームウェア更新",
	"Atualizar",
    "Bijwerken",
    "Guncelleme",
	"펌웨어 업데이트",
	"Обновлять"
};

const char MMSG_SETTING_WIKI_UPDATE[12][44] PROGMEM = {
	"Wifi firmware Update",
	"更新Wifi固件",	     // 更新
	"Mise a jour Wi-Fi",
	"WLAN-Update",
	"Actualizacion Wi-Fi",
	"Aggiornamento Wi-Fi",
	"Wifiファームウェア更新",
	"Atualizacao de Wi-Fi",
    "Wifi-firmware-update",
    "Wi-Fi Guncellemesi",
	"와이파이 펌웨어 업데이트",
	"Обновление WiFi"
};				
				
const char MMSG_IAP_FILE_FOUND[12][28] PROGMEM = {				
	"flash.wfm found",
	"flash.wfm已找到",
	"flash.wfm trouve",
	"flash.wfm gefunden",
	"flash.wfm encontrado",
	"flash.wfm trovato",
	"flash.wfm 見つかった",
	"flash.wfm found",
	"flash.wfm gevonden",
	"flash.wfm bulundu",
	"flash.wfm 설립하다",
	"flash.wfm найден"
};

const char MMSG_IAP_FILE_NOTFOUND[12][34] PROGMEM = {
	"flash.wfm not found",
	"flash.wfm未找到",
	"flash.wfm manquant",
	"flash.wfm fehlt",
	"falta flash.wfm",
	"flash.wfm mancante",
	"flash.wfm 見つかりません",
	"flash.wfm ausente",
	"flash.wfm ontbreekt",
	"flash.wfm eksik",
	"flash.wfm 찾을 수 없음",
	"flash.wfm отсутствует"
};

const char MMSG_IAP_FILE_ERROR[12][26] PROGMEM = {
	"file error",
	"文件错误",
	"erreur de fichier",
	"Dateifehler",
	"error de archivo",
	"errore di file",
	"ファイルエラー",
	"erro de arquivo",
	"bestandsfout",
	"dosya hatasi",
	"파일 오류",
	"ошибка файла"
};

const char MMSG_IAP_FILE_LOADED[12][40] PROGMEM = {
	"file loaded",
	"文件已载入",	
	"fichier charge",
	"Datei geladen",
	"archivo cargado",
	"file caricato",
	"ファイルが読み込まれ",
	"arquivo carregado",
	"bestand geladen",
	"dosya yuklendi",
	"로드된 파일",
	"файл загружен"
};

const char MMSG_IAP_NONEED[12][30] PROGMEM = {
	"No update",
	"无更新",		
	"Pas de mise a jour",
	"Kein Update",
	"Ninguna actualizacion",
	"Nessun aggiornamento",
	"更新なし",
	"Sem atualizacao",
	"Geen update",
	"Guncelleme yok",
	"업데이트 없음",
	"Без обновления"
};

const char MMSG_MACHINE_NOMATCH[12][37] PROGMEM = {
	"Error:incompatible",
	"错误: 设备不匹配",
	"Erreur:incompatible",
	"Fehler:Inkompatibel",
	"Error:incompatible",
	"Errore:incompatibile",
	"エラー:設備不一致",
	"Erro:incompat vel",
	"Fout:incompatibel",
	"Hata:uyumsuz",
	"오류: 기기 불일치",
	"несовместимо"
};


const char MMSG_ENDSTOP_PROBE[12][24] PROGMEM = {
	"Z-Probe: ",
	"调平传感器: ",		
	"Sonde Z: ",
	"Z-Sonde: ",
	"Sonda Z: ",
	"Sonda-Z: ",
	"Zプローブ: ",
	"Sonda-Z: ",
	"Z-Probe: ",
	"Z-Probe: ",
	"레벨 센서: ",
	"Z-Probe: ",
};

const char MMSG_QUIET_MODE[12][25] PROGMEM = {
	"Quiet Mode",
	"静音打印模式",		
	"Mode silencieux",
	"Ruhemodus",
	"Modo silencioso",
	"Modalita silenziosa",
	"マナーモード",
	"Modo silencioso",
	"Stille modus",
	"Sessiz mod",
	"무음 모드",
	"Тихо режим",
};

const char MMSG_POWERLOSS_SET[12][15] PROGMEM = {
	"Powerloss",
	"断电保护",		
	"Powerloss",
	"Powerloss",
	"Powerloss",
	"Powerloss",
	"Powerloss",
	"Powerloss",
	"Powerloss",
	"Powerloss",
	"Powerloss",
	"Powerloss",
};

const char MMSG_ERROR[12][14] PROGMEM = {
	"Error",
	"错误",
	"Erreur",
	"Fehler",
	"Error",
	"Errore",
	"Error",
	"Erro",
	"Fout",
	"Hata",
	"오류",
	"Ошибка",
};


#endif // 
