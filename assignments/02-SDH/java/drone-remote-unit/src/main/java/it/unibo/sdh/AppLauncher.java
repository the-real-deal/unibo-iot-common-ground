package it.unibo.sdh;

import org.slf4j.Logger;
import org.slf4j.LoggerFactory;

import it.unibo.sdh.impl.controller.DashboardController;
import it.unibo.sdh.impl.view.DashboardView;

public class AppLauncher {
    private static final Logger logger = LoggerFactory.getLogger(AppLauncher.class);
    public static void main(final String[] args) {
        final var serialPort = args.length > 0 ? args[0] : "/dev/cu.usbmodem1201";
        final var dashboardView = new DashboardView();
        final var dashboardController = new DashboardController(serialPort, dashboardView);
        
        dashboardView.registerController(dashboardController);
        dashboardView.display();

        logger.atInfo().log("Application has successfully started!");
    }
}
