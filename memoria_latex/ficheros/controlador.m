% - Script PID.m
%  - 
%  - Contiene el código para poder ajustar el controlador PID y
%  - realizar la discretización de éste.
%  - Nombre: Juan Carlos Calvo Sansegundo.  Fecha: 04/07/2017.

% Paso 0: Carga de la función de transferencia estimada.

clc, clear;
load Exp6_18a32
H_num = datos.best.num;
H_den = datos.best.den;
H = tf(H_num,H_den);

[H_ceros,H_polos,H_ganancia]=tf2zp(H_num,H_den);

% Definición del vector de tiempos.
Ts = 10;                     
t_final =4000;
t = 0:Ts:t_final;   
s_ref = ones(1,t_final/Ts + 1);

% Respuesta al escalón
lsim(H,s_ref,t);
grid on;
clear datos;

% Paso 1. Diagrama de bode la planta.

[MG,MF,WCG,WCF]=margin(H_num,H_den);
bode(H,{10^-6,10^6});
grid on;
hold off;

% Paso 2. Diagrama de nyquist de la planta

% Definición de la circunferencia unidad.
theta = 0:0.01:2*pi;
x = cos(theta);
y = sin(theta);
figure;
plot(x,y);

% Dibujar los polos y ceros del sistema.
n_polos = size(H_polos);
polos = ones([2 n_polos]);
for i=1:n_polos
    polos(1,i)=real(H_polos(i));
    polos(2,i)=imag(H_polos(i));
end;

n_ceros = size(H_ceros);
ceros = ones([2 n_ceros]);
for i=1:n_ceros
    ceros(1,i)=real(H_ceros(i));
    ceros(2,i)=imag(H_ceros(i));
end;

hold on
plot(polos(1,:),polos(2,:),'xr');
plot(ceros(1,:),ceros(2,:),'or');

% Inclusión del diagrama de Nyquist.
nyquist(H);
axis([-10^-3 10^-3 -10^-3 10^-3]);
hold off;

% Paso 3. Definición del controlador PID y sistema realimentado.
Kp = 28;   % Ganancia proporcional.
Ki = 0.037;   % Ganancia integral.
Kd = 300;     % Ganancia derivativa.
td = Kd/Kp;   % Tiempo integral.
ti = Kp/Ki;   % Tiempo derivativo.

Gc_num =[td*ti ti 1];
Gc_den =[ti 0];
Gc = Kp*tf(Gc_num,Gc_den);

% Definición de las funciones de transferencia del sistema.
GLa = Gc*H;   % F.T de lazo abierto.
GD  = Gc*H;   % F.T de lazo directo.
GLc = tf(GLa.num{1},GLa.num{1}+GLa.den{1}); % F.T de lazo cerrado.

step(GLc,t);
grid on;

% Paso 4: Estabilidad del sistema con lazo cerrado.

% Dibujo de la circunferencia unidad.
figure;
plot(x,y,'g');

% Obtención y dibujo de polos y ceros.
[GLc_ceros,GLc_polos,GLc_ganancia]=tf2zp(GLc.num{1},GLc.den{1});

n_polos = size(GLc_polos);
polos = ones([2 n_polos]);
for i=1:n_polos
    polos(1,i)=real(GLc_polos(i));
    polos(2,i)=imag(GLc_polos(i));
end;

n_ceros = size(GLc_ceros);
ceros = ones([2 n_ceros]);
for i=1:n_ceros
    ceros(1,i)=real(GLc_ceros(i));
    ceros(2,i)=imag(GLc_ceros(i));
end;

hold on
plot(polos(1,:),polos(2,:),'xr');
plot(ceros(1,:),ceros(2,:),'or');

p = 1 + Gc*H;
nyquist(Gc*H);
hold off;

% Paso 5. Discretización del controlador PID y comparativa
%  con la respuesta en tiempo continuo.

GLc_ss = ss(GLc);
GLcd_tus = c2d(GLc_ss,10,'tustin'); % Tustin;

step(GLc)
hold on
step(GLcd_tus)
hold off

title('Comparativa respuesta en tiempo continuo y en tiempo discreto')
legend('Resp tiempo continuo','Resp tiempo discreto');
grid on;