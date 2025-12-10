package it.unibo.sdh.impl.dashboard.view;

import java.awt.Dimension;
import java.awt.event.WindowAdapter;
import java.awt.event.WindowEvent;

import javax.swing.Box;
import javax.swing.BoxLayout;
import javax.swing.JButton;
import javax.swing.JFrame;
import javax.swing.JLabel;
import javax.swing.JPanel;
import javax.swing.JTextField;
import javax.swing.SwingUtilities;

import it.unibo.sdh.impl.dashboard.controller.DashboardController;

public class DashboardView extends JFrame {
    
    private JButton takeOffButton;
    private JButton landButton;
    private JTextField distanceDisplay;
    
    private DashboardController controller;

    public DashboardView() {
        super(".:: Drone Remote Unit — DASHBOARD ::.");

        setSize(600,150);
		this.setResizable(false);
		
		JPanel mainPanel = new JPanel();
		mainPanel.setLayout(new BoxLayout(mainPanel, BoxLayout.Y_AXIS));
		mainPanel.add(Box.createRigidArea(new Dimension(0,20)));
		
		JPanel infoLine = new JPanel();
		infoLine.setLayout(new BoxLayout(infoLine, BoxLayout.LINE_AXIS));
		distanceDisplay = new JTextField("-- mt.");
		distanceDisplay.setEditable(false);
		distanceDisplay.setPreferredSize(new Dimension(150,15));
		infoLine.add(new JLabel("Distance to ground: "));
		infoLine.add(distanceDisplay);
		
		mainPanel.add(infoLine);
		mainPanel.add(Box.createRigidArea(new Dimension(0,20)));
		mainPanel.setPreferredSize(new Dimension(200,20));
        
		takeOffButton = new JButton("Take Off");
		takeOffButton.setEnabled(false);
		takeOffButton.addActionListener((l) -> {
            
        });

		landButton = new JButton("Land");
		landButton.setEnabled(false);
		landButton.addActionListener((l) -> {
            
        });
		JPanel buttonPanel = new JPanel();
		buttonPanel.setLayout(new BoxLayout(buttonPanel, BoxLayout.X_AXIS));	    
		buttonPanel.add(takeOffButton);
		buttonPanel.add(landButton);
		
		mainPanel.add(buttonPanel);
		mainPanel.add(Box.createRigidArea(new Dimension(0,20)));
		setContentPane(mainPanel);

        addWindowListener(new WindowAdapter() {
            public void windowClosing(WindowEvent ev){
				System.exit(-1);
			}
        });
    }

    public void display() {
        SwingUtilities.invokeLater(() -> {
            this.setVisible(true);
        });
    }

    public void registerController(DashboardController controller) {
        this.controller = controller;
    }
}
