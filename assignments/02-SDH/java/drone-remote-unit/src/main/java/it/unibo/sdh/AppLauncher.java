package it.unibo.sdh;

import it.unibo.sdh.impl.controller.DashboardController;
import it.unibo.sdh.impl.view.DashboardView;

public class AppLauncher {
    public static void main(String[] args) {

        final var serialPort = "/dev/cu.usbmodem1201";
        final var dashboardView = new DashboardView();
        final var controller = new DashboardController(serialPort, dashboardView);
        
        dashboardView.registerController(controller);
        dashboardView.display();
    }
}
