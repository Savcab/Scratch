import java.util.ArrayList;

public class TxHandler {

    
    public UTXOPool data;
    /**
     * Creates a public ledger whose current UTXOPool (collection of unspent transaction outputs) is
     * {@code utxoPool}. This should make a copy of utxoPool by using the UTXOPool(UTXOPool uPool)
     * constructor.
     */
    public TxHandler(UTXOPool utxoPool) {
        this.data = new UTXOPool(utxoPool);
    }

    /**
     * @return true if:
     * (1) all outputs claimed by {@code tx} are in the current UTXO pool, 
     * (2) the signatures on each input of {@code tx} are valid, 
     * (3) no UTXO is claimed multiple times by {@code tx},
     * (4) all of {@code tx}s output values are non-negative, and
     * (5) the sum of {@code tx}s input values is greater than or equal to the sum of its output
     *     values; and false otherwise.
     */
    public boolean isValidTx(Transaction tx) {
        if(tx == null){
            return false;
        }
        //checks (1)

        //checks (2)
        if(!signaturesValid(tx)){
            return false;
        }
        //checks (3)
        if(!noDuplicateConsumptionOfUTXO(tx)){
            return false;
        }
        //checks (4) and (5)
        if(!inputsOutputsValuesValid(tx)){
            return false;
        }

        return true;
    }

    /**
     * Helper function that checks to see if the signatures on a list of inputs are valid
     * @param tx
     * @return
     */
    private boolean signaturesValid(Transaction tx){
        for(int i = 0 ; i < tx.numInputs() ; i++){
            UTXO temp = new UTXO(tx.getInput(i).prevTxHash, tx.getInput(i).outputIndex);
            Transaction.Output prev = data.getTxOutput(temp);
            if(prev == null){
                return false;
            }
            if(!Crypto.verifySignature(prev.address, tx.getRawDataToSign(i), tx.getInput(i).signature)){
                return false;
            }
        }
        return true;
    }

    /**
     * Helper function that verifies that the inputs and outputs are valid, meaning:
     *      Sum of input amount is more than or equal to output amount
     *      Output values are not negative
     * returns false otherwise
     * @param tx
     * @return
     */
    private boolean inputsOutputsValuesValid(Transaction tx){
        ArrayList<Transaction.Input> inputs = tx.getInputs();
        ArrayList<Transaction.Output> outputs = tx.getOutputs();
        int inputSum = 0;
        int outputSum = 0;
        for(Transaction.Output output : outputs){
            if(output.value < 0){
                //output value negative
                return false;
            }
            outputSum += output.value;
        }
        for(Transaction.Input input : inputs){
            UTXO temp = new UTXO(input.prevTxHash, input.outputIndex);
            Transaction.Output prev;
            if(data.contains(temp)){
                prev = data.getTxOutput(temp);
                inputSum += prev.value;
            } else {
                //input's previous hash isn't in UTXOPool
                return false;
            }
        }
        if(inputSum < outputSum){
            //output is more than input
            return false;
        }
        return true;
    }

    /**
     * Helper function for isValidTx
     * Returns true if there are no duplicated UTXO that is consumed
     * Returns false otherwise
     * @param tx
     * @return
     */
    private boolean noDuplicateConsumptionOfUTXO(Transaction tx){
        for(int i = 0 ; i < tx.numInputs()-1 ; i++){
            Transaction.Input input = tx.getInput(i); 
            UTXO curr = new UTXO(input.prevTxHash, input.outputIndex);
            for(int j = i+1 ; j < tx.numInputs() ; j++){
                Transaction.Input input2 = tx.getInput(j);
                UTXO other = new UTXO(input2.prevTxHash, input2.outputIndex);
                if(curr.hashCode() == other.hashCode()){
                    return false;
                }
            }
        }
        return true;
    }


    /**
     * Handles each epoch by receiving an unordered array of proposed transactions, checking each
     * transaction for correctness, returning a mutually valid array of accepted transactions, and
     * updating the current UTXO pool as appropriate.
     */
    public Transaction[] handleTxs(Transaction[] possibleTxs) {
        ArrayList<Transaction> valid = new ArrayList<Transaction>();
        for(int i = 0 ; i < possibleTxs.length ; i++){
            if(isValidTx(possibleTxs[i])){
                deleteCoins(possibleTxs[i]);
                addCoins(possibleTxs[i]);
                valid.add(possibleTxs[i]);
            }
        }
        Transaction[] ans = new Transaction[valid.size()];
        ans = valid.toArray(ans);
        return ans;
    }

    /**
     * Helper function for handleTxs that adds all the created UTXOs from a transaction to the UTXOPool
     */
    private void addCoins(Transaction tx){
        for(int i = 0; i < tx.numOutputs(); i++){
            UTXO temp = new UTXO(tx.getHash(), i);
            data.addUTXO(temp, tx.getOutput(i));
        }
    }

    /**
     * Helper function for handlTxs that deletes all the consumed coins (UTXO) of a transaction from UTXOPool
     */
    private void deleteCoins(Transaction tx){
        for(int i = 0 ; i < tx.numInputs() ; i++){
            UTXO temp = new UTXO(tx.getInput(i).prevTxHash, tx.getInput(i).outputIndex);
            data.removeUTXO(temp);
        }
    }

}
