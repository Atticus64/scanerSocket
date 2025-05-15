# 📘 Server para WebSocket y Escaner Card NFC App

## Entorno de desarrollo

### Pasos para iniciar servidor

1. Preparar entorno virtual
```bash
py -3 -m venv .venv
```
2. Cargar entorno virtual
```bash
# en powershell
./.venv/Scripts/Activate.ps1
# en cmd
./.venv/Scripts/activate.bat
```
3. Instalar dependencias
```bash
pip install -r requirements.txt
```

> [!TIP]
> Si agregas nuevas dependencias durante el desarrollo, recuerda:
> Instálalas localmente con pip install <paquete>.
> Actualiza el archivo con:
> ```bash
> pip freeze > requirements.txt
> ```

4. Ejecutar servidor en modo desarrollo
```bash
python server.py
```


## 🧪 Endpoints disponibles

Ruta| Método     |Descripción
-- |---| --
/ | GET | Prueba básica de saludo
/data |POST| Enviar datos al frontend simulando escaneo


## Ejecutar escaner NFC

```bash
python main.py
```