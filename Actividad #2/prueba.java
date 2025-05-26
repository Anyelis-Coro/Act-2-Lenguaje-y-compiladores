import java.security.MessageDigest;
import java.security.NoSuchAlgorithmException;
import java.util.ArrayList;
import java.util.List;
import java.util.Random;

class Nodo {
    String partida;
    List<Integer> cuerpo;
    String firmaDigital;

    public Nodo(String partida, List<Integer> cuerpo, String firmaDigital) {
        this.partida = partida;
        this.cuerpo = cuerpo;
        this.firmaDigital = firmaDigital;
    }

    @Override
    public String toString() {
        return "Nodo{\n" +
                "  partida='" + partida + "'\n" +
                "  cuerpo=" + cuerpo + "\n" +
                "  firmaDigital='" + firmaDigital + "'\n" +
                '}';
    }
}

public class prueba {
    private static final Random random = new Random();
    private static MessageDigest digest;

    static {
        try {
            digest = MessageDigest.getInstance("SHA-256");
        } catch (NoSuchAlgorithmException e) {
            throw new RuntimeException(e);
        }
    }

    public static void main(String[] args) {
        // Escenarios a probar
        int[][] escenarios = { { 3, 4 }, { 10, 200 }, { 200, 10 } };

        for (int[] escenario : escenarios) {
            int n = escenario[0];
            int k = escenario[1];

            System.out.println("\nEjecutando escenario: n=" + n + ", k=" + k);
            long inicio = System.currentTimeMillis();

            List<Nodo> lista = crearListaNodos(n, k);

            long fin = System.currentTimeMillis();
            long tiempoEjecucion = fin - inicio;

            System.out.println("Tiempo de ejecución: " + tiempoEjecucion + " ms");

            // Mostrar los primeros 3 nodos para verificación
            if (lista.size() > 0) {
                System.out.println("\nPrimer nodo:");
                System.out.println(lista.get(0));

                if (lista.size() > 1) {
                    System.out.println("\nSegundo nodo:");
                    System.out.println(lista.get(1));

                    if (lista.size() > 2) {
                        System.out.println("\nTercer nodo:");
                        System.out.println(lista.get(2));
                    }
                }
            }
        }
    }

    public static List<Nodo> crearListaNodos(int n, int k) {
        List<Nodo> lista = new ArrayList<>();
        String partidaAnterior = null;

        for (int i = 0; i < n; i++) {
            // Generar cuerpo con k elementos aleatorios
            List<Integer> cuerpo = generarCuerpo(k);

            // Determinar partida
            String partida;
            if (i == 0) {
                partida = generarPartidaInicial();
            } else {
                partida = partidaAnterior;
            }

            // Generar firma digital
            String firmaDigital = generarFirmaDigital(partida, cuerpo);

            // Crear nodo y añadir a la lista
            Nodo nodo = new Nodo(partida, cuerpo, firmaDigital);
            lista.add(nodo);

            // Actualizar partida anterior para el próximo nodo
            partidaAnterior = firmaDigital;
        }

        return lista;
    }

    private static List<Integer> generarCuerpo(int k) {
        List<Integer> cuerpo = new ArrayList<>();
        for (int i = 0; i < k; i++) {
            cuerpo.add(random.nextInt(100000) + 1); // Números entre 1 y 100000
        }
        return cuerpo;
    }

    private static String generarPartidaInicial() {
        String fechaHora = java.time.LocalDateTime.now().toString();
        return calcularSHA256(fechaHora);
    }

    private static String generarFirmaDigital(String partida, List<Integer> cuerpo) {
        StringBuilder sb = new StringBuilder(partida);
        for (Integer num : cuerpo) {
            sb.append(" ").append(num);
        }
        return calcularSHA256(sb.toString());
    }

    private static String calcularSHA256(String input) {
        byte[] hash = digest.digest(input.getBytes(java.nio.charset.StandardCharsets.UTF_8));
        StringBuilder hexString = new StringBuilder();

        for (byte b : hash) {
            String hex = Integer.toHexString(0xff & b);
            if (hex.length() == 1)
                hexString.append('0');
            hexString.append(hex);
        }

        return hexString.toString();
    }
}