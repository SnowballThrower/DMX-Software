/*
 * To change this license header, choose License Headers in Project Properties.
 * To change this template file, choose Tools | Templates
 * and open the template in the editor.
 */
package SnowballThrower.dmxsoftware.Database;

import java.util.List;

/**
 *
 * @author Sven
 */
class Condition {

    int Channel;
    List<Range> ranges;
    boolean met;

    boolean met(int[] values) {
        if (Channel > 0 && Channel < values.length) {
            for (int count = 0; count < ranges.size(); count++) {
                if ((values[Channel] >= ranges.get(count).min && values[Channel] <= ranges.get(count).max)) {
                    return true;
                }
            }
        }
        return false;
    }
}
